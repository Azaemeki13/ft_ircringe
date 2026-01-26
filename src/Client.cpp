/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:50:42 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/26 14:16:34 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : 
    socketFD(-1), _connexionTime(time(NULL)), isAuthorized(false),
    nickName(""),  userName(""), realName(""), hostName("")
{
    joinedChannels.clear();
}

Client::Client(int fd, const std::string &host) : 
    socketFD(fd), _connexionTime(time(NULL)), isAuthorized(false),
    nickName(""),  userName(""), realName(""), hostName(host)
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

std::string& Client::getNickName() const
{
    return nickName;
}

std::string& Client::getUserName() const
{
    return userName;
}

std::string& Client::getRealName() const
{
    return realName;
}

std::string& Client::getHostName() const
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

void Client::setBuffer(const std::string &bufer)
{
    buffer = bufer;
}
