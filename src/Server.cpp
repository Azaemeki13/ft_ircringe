/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 13:18:13 by cauffret          #+#    #+#             */
/*   Updated: 2026/01/21 10:09:09 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// OCF 

Server::Server() : port(66)
{
    initServ(66);
    std::cout << "Default constructor called for Server." << std::endl;
}

Server::Server(int port)
{
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
    // then I need to set my options for my socket, the fact I can reuse adress if spamming ./myprog 
    int opt = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
        throw std::runtime_error("Failed to set socket option: " + std::string(strerror(errno)));
    int v6only = 1;
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
    if (listen(listener, 10) < 0)
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
    std::cout << "Server is running (Epoll mode) ... " << std::endl;
    while(true)
    {
        event_count = epoll_wait(epfd, events, MAX_EVENTS, - 1);
        if (event_count == -1)
        {
            if (errno == EINTR)
                continue;            
            throw std::runtime_error("Epoll wait failed.");
        }
        for (int i = 0 ; i < event_count; ++i)
        {
            int current_fd = events[i].data.fd;
            if (current_fd == listener)
                handleNewConnection();
            else
                handleClientMessage(current_fd);
        }
    }
}

void Server::handleNewConnection()
{
    struct sockaddr_in6 client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int new_fd = accept(listener, (struct sockaddr *) &client_addr, &addr_len);
    if (new_fd == -1)
    {
        std::cerr << "Error accepting connection" << std::endl; // maybe unique exception here
        return;
    }
    char client_ip[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &client_addr.sin6_addr, client_ip, INET6_ADDRSTRLEN);
    std::cout << "Accepted connection from: " << client_ip << std::endl;
    fcntl(new_fd, F_SETFL, O_NONBLOCK);
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
    }
    else
    {
        std::cout << "Client: " << fd << ": " << buffer << std::endl;
    }
}

const char *Server::warnRunning::what() const throw()
{
    return("Warning on runtime: ");
}