/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:41:34 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/04 17:48:12 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void modeI(const Client &client, Channel* channel, bool add)
{
    if(channel->isOperator(client.getSocketFD()))
    {
        channel->setInviteOnly(add);
        std::string mode;
        if(add)
            mode = "+i";
        else
            mode = "-i";
        std::string modeMsg = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + 
            " MODE " + channel->getChannelName() + " " + mode + "\r\n";
        std::vector<int> &channelClients = channel->getClients();
        std::vector<int>::iterator it = channelClients.begin();
        while (it != channelClients.end())
        {
            send(*it, modeMsg.c_str(), modeMsg.length(), 0);
            ++it;
        }
    }
    else
        throw Server::warnRunning(client.getSocketFD(), 482);
}

void modeT(const Client &client, Channel* channel, bool add)
{
    if(channel->isOperator(client.getSocketFD()))
    {
        channel->setTopicProtected(add);
        std::string mode;
        if(add)
            mode = "+t";
        else
            mode = "-t";
        std::string modeMsg = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + 
            " MODE " + channel->getChannelName() + " " + mode + "\r\n";
                std::vector<int> &channelClients = channel->getClients();
        std::vector<int>::iterator it = channelClients.begin();
        while (it != channelClients.end())
        {
            send(*it, modeMsg.c_str(), modeMsg.length(), 0);
            ++it;
        }
    }
    else
        throw Server::warnRunning(client.getSocketFD(), 482);
}

void modeK(const Client &client, Channel* channel, bool add, const Commands &command, std::string key)
{
    (void)command;
    if(channel->isOperator(client.getSocketFD()))
    {
        std::string mode;
        if(add)
        {
            if(key.empty())
                throw Server::warnRunning(client.getSocketFD(), 461);
            channel->setKey(key);
            mode = "+k " + key;
        }
        else
        {
            channel->setKey("");
            mode = "-k";
        }
        std::string modeMsg = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + 
            " MODE " + channel->getChannelName() + " " + mode + "\r\n";
        std::vector<int> &channelClients = channel->getClients();
        std::vector<int>::iterator it = channelClients.begin();
        while (it != channelClients.end())
        {
            send(*it, modeMsg.c_str(), modeMsg.length(), 0);
            ++it;
        }
    }
    else
        throw Server::warnRunning(client.getSocketFD(), 482);
}

/*void modeO(Channel* channel, bool add)
void modeL(Channel* channel, bool add)*/

void mode(Server &server, Client &client, const Commands &command)
{
    if (command.params.size() < 2)
        throw Server::warnRunning(client.getSocketFD(), 461);
    
    std::string channelName = command.params[0];
    std::string modeStr = command.params[1];
    
    std::map<std::string, Channel>::iterator it = server.getChannels().find(channelName);
    if (it == server.getChannels().end())
        throw Server::warnRunning(client.getSocketFD(), 403);
    
    Channel* channel = &(it->second);
    
    if (!channel->isInChannel(client.getSocketFD()))
        throw Server::warnRunning(client.getSocketFD(), 442);
    
    bool add = true;
    size_t i = 0;
    while (i < modeStr.length())
    {
        switch (modeStr[i])
        {
            case '+':
                add = true;
                break;
            case '-':
                add = false;
                break;
            case 'i':
                modeI(client, channel, add);
                break;
            case 't':
                modeT(client, channel, add);
                break;
            case 'k':
            {
                std::string key = "";
                if (command.params.size() > 2 && add)
                    key = command.params[2];
                modeK(client, channel, add, command, key);
                break;
            }
        }
        i++;
    }
}
