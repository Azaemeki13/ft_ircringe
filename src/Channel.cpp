/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:52:44 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/04 09:46:52 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() : channelName(""), topic(""), inviteOnly(0),
        topicProtected(0), key(""), userLimit(0)
{
    clients.clear();
    operators.clear();
}

Channel::Channel(const std::string& channelName) : channelName(channelName), topic(""), inviteOnly(false),
    topicProtected(false), key(""), userLimit(0)
{
    clients.clear();
    operators.clear();
}

Channel::Channel(const std::string& channelName, const std::string &password) : channelName(channelName), topic(""), inviteOnly(false),
    topicProtected(false), key(password), userLimit(0)
{
    clients.clear();
    operators.clear();
}

Channel::~Channel()
{
    std::cout << "Destructor called on channel." << std::endl;
}

std::vector<int> &Channel::getClients()
{
    return(clients);
}

const std::vector<int> &Channel::getOperators() const
{
    return(operators);
}


std::vector<int> &Channel::getOperators()
{
    return(operators);
}

const std::string &Channel::getTopic() const
{
    return topic;
}

void Channel::setTopic(const std::string &topic)
{
    this->topic = topic;
}

const std::string Channel::getKey() const
{
    return key;
}

bool Channel::isInviteOnly() const
{
    return inviteOnly;
}

bool Channel::isTopicProtected() const
{
    return topicProtected;
}

int Channel::getUserLimit() const
{
    return userLimit;
}

void Channel::addClient(Client* client)
{
    clients.push_back(client->getSocketFD());
}

void Channel::addOperator(Client* client)
{
    operators.push_back(client->getSocketFD());
}

void Channel::removeClient(Client* client)
{
    std::vector<int>::iterator it = clients.begin();
    while (it != clients.end())
    {
        if (*it == client->getSocketFD())
        {
            clients.erase(it);
            return;
        }
        ++it;
    }
}

void Channel::removeClient(int client_fd)
{
    std::vector<int>::iterator it = clients.begin();
    while (it != clients.end())
    {
        if (*it == client_fd)
        {
            clients.erase(it);
            return;
        }
        ++it;
    }
}

void Channel::removeOperator(Client* client)
{
    std::vector<int>::iterator it = operators.begin();
    while (it != operators.end())
    {
        if (*it == client->getSocketFD())
        {
            operators.erase(it);
            return;
        }
        ++it;
    }
}

void Channel::removeOperator(int client)
{
    std::vector<int>::iterator it = operators.begin();
    while (it != operators.end())
    {
        if (*it == client)
        {
            operators.erase(it);
            return;
        }
        ++it;
    }
}

bool Channel::isInChannel(int client) const
{
    for (std::vector<int>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (*it == client)
            return(1);
    }
    return(0);
}

void Channel::broadcastMessage(Client* fromWho, const std::string &msg)
{
    std::vector<int>::iterator it = clients.begin();
    while(it != clients.end())
    {
        if (*it != fromWho->getSocketFD())
            send(*it, msg.c_str(), msg.length(), 0);
        ++it;
    }
}

bool Channel::isOperator(const int client) const
{
    for(std::vector<int>::const_iterator it = operators.begin(); it != operators.end() ; ++it)
    {
        if (*it == client)
            return(true);
    }
    return(false);
}