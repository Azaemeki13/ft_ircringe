/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_kick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:10:55 by cauffret          #+#    #+#             */
/*   Updated: 2026/02/04 10:10:11 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// pouvoir kick qqun d'un channel
// KICK <channel> <nick> [<reason>]

void kick(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() < 2)
        throw Server::warnJoin(461, client.getSocketFD(), "KICK "); //  	ERR_CANNOTDOCOMMAND 
    std::string channel_name = command.params[0];
    std::string reason = " :Kicked by operator";
    if (command.params.size() >= 3)
        reason = " :" + command.params[2];
    std::map<std::string, Channel>::iterator chan_it = server.getChannels().find(channel_name);
    if (chan_it == server.getChannels().end())
        throw Server::warnJoin(403, client.getSocketFD(), channel_name);
    Channel& channel = chan_it->second;
    if (!channel.isOperator(client.getSocketFD()))
        throw Server::warnJoin(482, client.getSocketFD(), channel_name);
    if (!channel.isInChannel(client.getSocketFD()))
        throw Server::warnJoin(442, client.getSocketFD(), channel_name);
    std::vector<std::string> targets = split(command.params[1], ',');
    std::vector<std::string>::iterator name_it;
    for (name_it = targets.begin(); name_it != targets.end(); ++name_it)
    {
        int victim = server.getClientByNickname(*name_it, 1);
        if (victim == -1)
        {
            server.sendError(client, "401", *name_it);
            continue;
        }
        if (!channel.isInChannel(victim))
            throw Server::warnJoin(441, client.getSocketFD(), *name_it + " " + channel_name);
        std::stringstream ss;
        ss << ":" << client.getNickName() << "!" << client.getUserName() << "@"  << client.getHostName() << " KICK" << channel_name << " " << *name_it << reason << "\r\n";
        std::string kickMsg = ss.str();
        channel.broadcastMessage(&client, kickMsg);
        if (channel.isOperator(victim))
            channel.removeOperator(victim);
        channel.removeClient(victim);
    }
}