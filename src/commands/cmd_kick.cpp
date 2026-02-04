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
        throw Server::warnJoin(client.getSocketFD(), 461, "KICK "); //  	ERR_CANNOTDOCOMMAND 
    std::string channel_name = command.params[0];
    std::string reason = " :Kicked by operator";
    if (command.params.size() >= 3 && !command.params[2].empty())
    {
        // CASE A: The parser split the reason (e.g., Netcat: KICK #chan user Bad Behavior)
        // We must glue params[2], params[3], etc. back together.
        if (command.params.size() > 3)
        {
            for (size_t i = 0; i < command.params.size(); i++)
            {
                std::cout << command.params[i] << " ";
            }
            reason = " :" + command.params[2];
            for (size_t i = 3; i < command.params.size(); i++)
            {
                reason += " " + command.params[i];
            }
        }
        // CASE B: The parser handled it as one chunk (e.g., Srain: KICK #chan user :Bad Behavior)
        else
        {
            reason = " :" + command.params[2];
        }
    }
    std::map<std::string, Channel>::iterator chan_it = server.getChannels().find(channel_name);
    if (chan_it == server.getChannels().end())
        throw Server::warnJoin(client.getSocketFD(), 403, channel_name);
    Channel& channel = chan_it->second;
    if (!channel.isOperator(client.getSocketFD()))
        throw Server::warnJoin(client.getSocketFD(), 482, channel_name);
    if (!channel.isInChannel(client.getSocketFD()))
        throw Server::warnJoin(client.getSocketFD(), 442, channel_name);
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
            throw Server::warnJoin(client.getSocketFD(), 441, *name_it + " " + channel_name);
        std::stringstream ss;
        ss << ":" << client.getNickName() << "!" << client.getUserName() << "@"  << client.getHostName() << " KICK " << channel_name << " " << *name_it << reason << "\r\n";        std::string kickMsg = ss.str();
        channel.broadcastMessage(&client, kickMsg);
        send(client.getSocketFD(), kickMsg.c_str(), kickMsg.length(), 0);
        if (channel.isOperator(victim))
            channel.removeOperator(victim);
        channel.removeClient(victim);
    }
}