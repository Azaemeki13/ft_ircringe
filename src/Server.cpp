/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 13:18:13 by cauffret          #+#    #+#             */
/*   Updated: 2026/02/06 14:00:51 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.hpp"

// OCF 

Server::Server() : port(66)
{
    channels.insert(std::make_pair("general", Channel("general")));
    initServ(66);
    std::cout << "Default constructor called for Server." << std::endl;
}

Server::Server(int port)
{
    channels.insert(std::make_pair("general", Channel("general")));
    initServ(port);
    std::cout << "Parameterised constructor called for Server." << std::endl;
}

Server::Server(int port, std::string password) : password(password)
{
    channels.insert(std::make_pair("general", Channel("general")));
    initServ(port);
    std::cout << "Parameterised constructor called for Server." << std::endl;
}

Server::~Server()
{
    if (listener != -1)
        close(listener);
    if (epfd != -1)
        close (epfd);
    std::cout << "Called destructor on server." << std::endl;
}

void Server::initServ(int port)
{
    this->port = port;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    if ((listener = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
    fcntl(listener, F_SETFL, O_NONBLOCK);
    // then I need to set my options for my socket, the fact I can reuse adress if spamming ./myprog 
    int opt = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
        throw std::runtime_error("Failed to set socket option: " + std::string(strerror(errno)));
    int v6only = 0;
    if (setsockopt(listener, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only)) != 0)
        throw std::runtime_error("Failed to set socket option: " + std::string(strerror(errno)));
    serv_addr.sin6_family = AF_INET6;
    if (port != 0)
        serv_addr.sin6_port = htons(port);
    else    
        serv_addr.sin6_port = htons(PORT);
    serv_addr.sin6_addr = in6addr_any;
    if(bind(listener, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) // cast here not in ipv6 struct because bind is unvisersal
        throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
    if (listen(listener,  SOMAXCONN) < 0)
        throw std::runtime_error("Listened failed: " + std::string(strerror(errno)));
    if ((epfd = epoll_create1(0))  == -1)
        throw std::runtime_error("epoll_create1 failed.");
    fd_ev.events = EPOLLIN;
    fd_ev.data.fd = listener;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &fd_ev) == -1)
        throw std::runtime_error("Failed to add listener to epoll.");
    std::cout << "Server Listening on TCP port " << (port != 0 ? port : PORT) << std::endl;
}

void Server::run()
{
    int event_count;

    event_count = epoll_wait(epfd, events, MAX_EVENTS, 1);
    if (event_count == -1)
    {
        if (errno == EINTR)
            return;            
        throw std::runtime_error("Epoll wait failed.");
    }
    for (int i = 0 ; i < event_count; ++i)
    {
        int current_fd = events[i].data.fd;
        uint32_t current_events = events[i].events;
        if (current_fd == listener)
        {
            handleNewConnection();
            continue;
        }
        if (current_events & EPOLLIN)
            handleClientMessage(current_fd);
        if (current_events & EPOLLOUT)
            handleClientWrite(current_fd);
        if (current_events & (EPOLLERR | EPOLLHUP))
        {
            removeKickedClient(current_fd);
            epoll_ctl(epfd, EPOLL_CTL_DEL, current_fd, NULL);
            close(current_fd);
            clients.erase(current_fd);
        }
        std::cout << "FD" << current_fd << "HAS BEEN FULLY PURGED MOUAHAHA" << std::endl;
    }
}

void Server::handleNewConnection()
{
    struct sockaddr_in6 client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int new_fd;

    new_fd = accept(listener, (struct sockaddr *) &client_addr, &addr_len);
    if (new_fd == -1)
    {
        std::cerr << "Error accepting connection" << std::endl; // maybe unique exception here
        return;
    }
    char client_ip[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &client_addr.sin6_addr, client_ip, INET6_ADDRSTRLEN);
    // std::cout << "Accepted connection from: " << client_ip << std::endl; DEBUG
    if (fcntl(new_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "fcntl" << std::endl;
        close(new_fd);
        return;
    }
    Client new_client(new_fd, client_ip);
    clients.insert(std::make_pair(new_fd, new_client));
    fd_ev.events = EPOLLIN;
    fd_ev.data.fd = new_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &fd_ev) == -1)
    {
        std::cerr << "Failed to add client to epoll" << std::endl;
        close(new_fd);
        clients.erase(new_fd);
    }
}

void Server::handleClientMessage(int fd)
{
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_read <= 0)
    {
        std::cout << "Client " << fd << " disconnected." << std::endl;
        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
        clients.erase(fd);
        return;
    }
    Client &currentUser = clients[fd];
    currentUser.getBuffer().append(buffer, bytes_read);
    if (currentUser.getBuffer().size() > 512)
    {
        if (fd == -1)
            throw (Server::warnRunning(fd, 401));
        std::string messageReady = "GET KICKED FOR FLOODING MEANIE :)\r\n";
        currentUser.addTowBuffer(messageReady);
        this->enableWriteEvent(fd);
        removeKickedClient(fd);
        shutdown(fd, SHUT_WR);
        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
        clients.erase(fd);
        return;
    }
    size_t pos = 0;
    while ((pos = currentUser.getBuffer().find('\n')) != std::string::npos)
    {
        std::string substr;
        if (pos > 0 && currentUser.getBuffer()[pos - 1] == '\r')
            substr = currentUser.getBuffer().substr(0, pos - 1);
        else
            substr = currentUser.getBuffer().substr(0, pos);
        if (!substr.empty())
        {
            std::cout << "Debug: " << fd << " sent " << substr << std::endl;
            processCommand(currentUser, substr);
        }
        currentUser.getBuffer().erase(0, pos + 1);
    }
}

void Server::handleClientWrite(int fd)
{
    std::map<int, Client>::iterator it = clients.find(fd);
    if (it == clients.end())
        return;
    Client &c = it->second;
    std::string &buffer = c.getWriteBuffer();
    if (buffer.empty())
    {
        disableWriteEvent(fd);
        return;
    }
    ssize_t bytes_sent = send(fd, buffer.c_str(), buffer.length(), 0);
    if (bytes_sent > 0)
    {
        buffer.erase(0, bytes_sent);
        if (buffer.empty())
            disableWriteEvent(fd);
    }
    else if (bytes_sent == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
    }
    else
        std::cerr << "Send failed." << std::endl;
}

void Server::disableWriteEvent(int fd)
{
    struct epoll_event ev;
    std::memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1)
        std::cerr << "epoll_ctl disable write." << std::endl;
}

void Server::enableWriteEvent(int fd)
{
    struct epoll_event ev;
    std::memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1)
        std::cerr << "epoll_ctl enable write." << std::endl;
}

Server::warnRunning::warnRunning(int fd, int code) : client_fd(fd), errorCode(code)
{
    std::stringstream ss;
    ss << "Error code " << errorCode << " on FD N-" << client_fd;
    message = ss.str();
}

int Server::warnRunning::getFD() const
{
    return(client_fd);
}

int Server::warnRunning::getErrorCode() const
{
    return(errorCode);
}

const char *Server::warnRunning::what() const throw()
{

    return(message.c_str());
}

Server::warnRunning::~warnRunning() throw()
{}


Server::warnJoin::warnJoin(int fd, int code, std::string channel) : client_fd(fd), errorCode(code), channel(channel)
{}

std::string Server::warnJoin::getChannelName() const
{
    return(channel);
}
int Server::warnJoin::getFD() const
{
    return(client_fd);
}

int Server::warnJoin::getErrorCode() const
{
    return(errorCode);
}

const char *Server::warnJoin::what() const throw()
{

    return(channel.c_str());
}

Server::warnJoin::~warnJoin() throw()
{}

void Server::removeKickedClient(int fd)
{
    std::map<std::string, Channel>::iterator it = channels.begin();
    while (it != channels.end())
    {
        it->second.removeClient(fd);
        it->second.removeOperator(fd);
        ++it;
    }
}

void Server::sendError(Client &client, const std::string &code, const std::string &message)
{
    std::string nickname;
    if (client.getNickName().empty())
        nickname = "*";
    else
        nickname = client.getNickName();
    std::string errorMsg = ":server Error code: " + code + " " + nickname + " " + message + "\r\n";
    client.addTowBuffer(errorMsg);
    this->enableWriteEvent(client.getSocketFD());
}

std::string Server::getPassword() const
{
    return password;
}

int Server::getClientByNickname(const std::string& nickname)
{
    std::map<int, Client>::iterator clit = clients.begin();
    for(; clit != clients.end(); ++clit)
    {
        if (clit->second.getNickName() == nickname)
            return clit->second.getSocketFD();
    }
    return(-1);
}

const std::string Server::getClientByFD(int fd)
{
    std ::string result = "";
    std::map<int, Client>::iterator clit = clients.find(fd);
    if (clit == getClients().end())
        return(result);
    else
        return (clit->second.getNickName());

}

Client *Server::getClientPoint(int fd)
{
    std::map<int, Client>::iterator it = clients.find(fd);
    if (it == getClients().end())
        return NULL;
    else
        return &(it->second);
}

void Server::setPassword(const std::string &pwd)
{
    password = pwd;
}

const std::map<int, Client>& Server::getClients() const
{
    return clients;
}

std::map<int, Client>& Server::getClients()
{
    return clients;
}

std::map<std::string, Channel>& Server::getChannels()
{
    return channels;
}

std::string Server::getErrorMessage(int code, const Commands &cmd) const
{
    switch(code)
    {
        case 401:
            return(cmd.params[0] + " :No such nick/channel");
        case 403:
            return(cmd.params[0] + " :No such channel");
        case 411:
            return(":No recipient given" + cmd.command);
        case 421:
            return(cmd.command + " :Unknown command");
        case 412:
            return(":No text to send");
        case 431: 
            return(":No nickname given");
        case 432:
        {
            if(!cmd.params.empty())
            {
                if(cmd.command == "NICK")
                    return(cmd.params[0] + " :Erroneous nickname");
                else if(cmd.command == "USER")
                    return(cmd.params[0] + " :Erroneous username");
            }
            return(":Erroneous input");
        }
        case 433:
            return(cmd.params[0] + " :Nickname is already in use");
        case 442:
            return(cmd.params[0] + " :You're not on that channel");
        case 451:
            return(":You have not registered");
        case 461:
            return(cmd.command + " :Not enough parameters");
        case 462:
            return(":Unauthorized command (already registered)");
        case 464:
            return(":Password incorrect");
        case 472:
            return(":Unknown MODE flag");
        case 482:
            return(cmd.params[0] + " :You're not channel operator");
        default:
            return(":Unknown error"); //debug
    }
}

void Server::handleError(const Server::warnRunning &e, Client &client, const Commands &cmd)
{
    std::stringstream ss;
    ss << e.getErrorCode();
    std::string errorCode = ss.str();

    std::string errorMessage(getErrorMessage(e.getErrorCode(), cmd));
    sendError(client, errorCode, errorMessage);
}

void Server::processCommand(Client &client, const std::string &message) //maybe would be better to use a switch case :P
{
    Commands cmd(message, client.getSocketFD());
    if (cmd.command.empty())
        return;

    std::cout << "Processing command: " << cmd.command << std::endl;
    try
    {
        if (cmd.command == "CAP")
            return;
        if (cmd.command == "PASS")
        {
            pass(*this, client, cmd);
            return;
        }
        if (!client.getIsAuthorized())
            throw Server::warnRunning(client.getSocketFD(), 451);
        
        if (cmd.command == "NICK")
            nick(*this, client, cmd);
        else if (cmd.command == "USER")
            user(*this, client, cmd);
        else if (cmd.command == "JOIN")
            join(*this, client, cmd);
        else if (cmd.command == "PRIVMSG")
            privmsg(*this, client, cmd);
        else if (cmd.command == "PING")
            pong(*this ,client, cmd);
        else if(cmd.command == "TOPIC")
            topic(*this, client, cmd);
        else if(cmd.command == "MODE")
            mode(*this, client, cmd);
        else if(cmd.command == "INVITE")
            invite(*this, client, cmd);
        else if (cmd.command == "QUIT")
        {
            std::cout << "Client requested QUIT" << std::endl;
            return;
        }
        else if (cmd.command == "KICK")
            kick(*this, client, cmd);
        else
        {
            std::cout << "Unknown command: " << cmd.command << std::endl;
            throw Server::warnRunning(client.getSocketFD(), 421);
        }
    }
    catch (Server::warnRunning &e)
    {
        handleError(e, client, cmd);
        return;
    }
    catch(Server::warnJoin &e)
    {
        handleExceptions(e, client, *this);
    }
}