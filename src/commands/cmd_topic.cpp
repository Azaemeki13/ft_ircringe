/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_topic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 14:24:56 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/04 16:58:25 by chsauvag         ###   ########.fr       */
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
        throw Server::warnRunning(client.getSocketFD(), 461); //not enough parameters
    std::string channelName = command.params[0];   
    std::map<std::string, Channel> &channels = server.getChannels();
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if(it == channels.end())
        throw Server::warnRunning(client.getSocketFD(), 403);
    Channel &channel = it->second;
    std::vector<int> &channelClients = channel.getClients();
    if(std::find(channelClients.begin(), channelClients.end(), client.getSocketFD()) == channelClients.end())
        throw Server::warnRunning(client.getSocketFD(), 442);
    if(command.params.size() == 1)
    {
        std::string msg;
        if(channel.getTopic().empty())
            msg = ":server 331 " + client.getNickName() + " " + channelName + " :No topic is set\r\n";
        else
            msg = ":server 332 " + client.getNickName() + " " + channelName + " :" + channel.getTopic() + "\r\n";
        client.addTowBuffer(msg);
        server.enableWriteEvent(client.getSocketFD());
        return;
    }
    if(channel.isTopicProtected())
    {
        std::vector<int> &operators = channel.getOperators();
        if(std::find(operators.begin(), operators.end(), client.getSocketFD()) == operators.end())
            throw Server::warnRunning(client.getSocketFD(), 482); // ERR_CHANOPRIVSNEEDED
    }
    std::string newTopic = command.params[1];
    channel.setTopic(newTopic);
    std::string topicMsg = ":" + client.getNickName() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    client.addTowBuffer(topicMsg);
    server.enableWriteEvent(client.getSocketFD());
    channel.broadcastMessage(server,client.getSocketFD(), topicMsg);
}