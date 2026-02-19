/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_invite.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:43:15 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/06 14:19:36 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

bool isReceiverFound(Server &server, const std::string &receiver);

void invite(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() < 2)
        throw Server::warnRunning(client.getSocketFD(), 461);
    
    std::string targetNick = command.params[0];
    std::string channelName = command.params[1];
    std::map<std::string, Channel>::iterator chanIt = server.getChannels().find(channelName);
    if (chanIt == server.getChannels().end())
        throw Server::warnRunning(client.getSocketFD(), 403);
    
    Channel* channel = &(chanIt->second);
    
    if(!channel->isInChannel(client.getSocketFD()))
        throw Server::warnJoin(client.getSocketFD(), 442, channelName);
    if(channel->isOperator(client.getSocketFD()) == false)
        throw Server::warnJoin(client.getSocketFD(), 482, channelName);
    if(!isReceiverFound(server, targetNick))
        throw Server::warnRunning(client.getSocketFD(), 401);
    
    int targetFD = server.getClientByNickname(targetNick);
    if(channel->isInChannel(targetFD))
        throw Server::warnRunning(client.getSocketFD(), 443);
    
    channel->addInvitedUser(targetFD);
    
    std::string invitingReply = ":server 341 " + client.getNickName() + " " + targetNick + " " + channelName + "\r\n";
    client.addTowBuffer(invitingReply);
    server.enableWriteEvent(client.getSocketFD());
    std::string inviteMsg = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " INVITE " + targetNick + " " + channelName + "\r\n";
    client.addTowBuffer(inviteMsg);
    server.enableWriteEvent(client.getSocketFD());
}
