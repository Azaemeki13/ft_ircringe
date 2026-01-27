/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:50:42 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/27 13:23:35 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : 
    socketFD(-1), _connexionTime(time(NULL)), isAuthorized(false),
    authState(0), nickName(""),  userName(""), realName(""), hostName("")
{
    joinedChannels.clear();
}

Client::Client(int fd, const std::string &host) : 
    socketFD(fd), _connexionTime(time(NULL)), isAuthorized(false),
    authState(0), nickName(""),  userName(""), realName(""), hostName(host)
{
    joinedChannels.clear(); //starts empty
}

Client::~Client()
{
    std::cout << "Destructor called on client." << std::endl;
}

int Client::getSocketFD() const
{
    return(socketFD);
}

bool &Client::getIsAuthorized() 
{
    return(isAuthorized);
}

std::string& Client::getNickName() 
{
    return nickName;
}

std::string& Client::getUserName() 
{
    return userName;
}

std::string& Client::getRealName() 
{
    return realName;
}

std::string& Client::getHostName() 
{
    return hostName;
}

std::string& Client::getBuffer()
{
    return buffer;
}

void Client::setNickName(const std::string &nick)
{
    nickName = nick;
}

void Client::setUserName(const std::string &user)
{
    userName = user;
}

void Client::setRealName(const std::string &real)
{
    realName = real;
}

void Client::setHostName(const std::string &host)
{
    hostName = host;
}
