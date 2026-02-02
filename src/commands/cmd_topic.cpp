/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_topic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 14:24:56 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/02 15:26:40 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*Command: TOPIC
   Parameters: <channel> [ <topic> ]

   The TOPIC command is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if this action is allowed for the user
   requesting it.  If the <topic> parameter is an empty string, the
   topic for that channel will be removed.*/

void topic(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() < 1)
        throw Server::warnRunning(client.getSocketFD(), 461);
    std::string channelName = command.params[0];   
    std::map<std::string, Channel> &channels = server.getChannels();
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if(it == channels.end())
        throw Server::warnRunning(client.getSocketFD(), 403);
    Channel &channel = it->second;
    std::vector<int> &channelClients = channel.getClients();
    std::string readyToPrint = ":server 332 " + client.getNickName() + " " + channelName + " :";
    bool isInChannel = false;
    std::vector<int>::iterator it2 = channelClients.begin();
    while(it2 != channelClients.end())
    {
        if(*it2 == client.getSocketFD())
        {
            isInChannel = true;
            break;
        }
        ++it2;
    }
    if(!isInChannel)
        throw Server::warnRunning(client.getSocketFD(), 442);
    if(command.params.size() == 1)
    {
        if(channel.getTopic().empty())
            readyToPrint += "No topic is set\r\n";
        else
            readyToPrint += channel.getTopic() + "\r\n";
        send(client.getSocketFD(), readyToPrint.c_str(), readyToPrint.length(), 0);
        return;
    }
}