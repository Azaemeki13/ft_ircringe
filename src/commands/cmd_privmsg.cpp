/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 10:42:59 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/06 11:03:39 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*Command: PRIVMSG
   Parameters: <receiver>{,<receiver>} <text to be sent>*/

//bool isValidReciever(Server &server, const std::string &receiver)

bool isReceiverFound(Server &server, const std::string &receiver)
{
    const std::map<int, Client> clients = server.getClients();
    const std::map<std::string, Channel> &channels = server.getChannels();
    
    std::string nickFirstChar = "[]{}^|_`-";
    std::string channelFirstChar = "&#";
    
    if(!receiver.empty())
    {
        if(std::isalpha(receiver[0]) || nickFirstChar.find(receiver[0]) != std::string::npos)
        {
            std::map<int, Client>::const_iterator it = clients.begin();
            while(it != clients.end())
            {
                if(it->second.getNickName() == receiver)
                return(true);
                it++;
            }
        }
        else if(channelFirstChar.find(receiver[0]) != std::string::npos)
        {
            if (channels.find(receiver) != channels.end())
            return(true);
        }
    }
    return(false);
}

void sendToUser(Server &server, Client &client, const std::string &receiver, const std::string &messageToSend)
{
    int recipientFD = server.getClientByNickname(receiver);
    if(recipientFD == -1)
        throw Server::warnRunning(client.getSocketFD(), 401);
    Client *recipient = &server.getClients()[recipientFD];
    std::string messageReady = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " PRIVMSG " + receiver + " :" + messageToSend + "\r\n";
    recipient->addTowBuffer(messageReady);
    server.enableWriteEvent(recipient->getSocketFD());
}

void sendToChannel(Server &server, Client &client, const std::string &channel, const std::string &messageToSend)
{
    std::map<std::string, Channel>::iterator channel_it = server.getChannels().find(channel);
    if (channel_it == server.getChannels().end())
        throw Server::warnJoin(client.getSocketFD(), 403, channel);
    if (channel_it->second.isInChannel(client.getSocketFD()) == false)
        throw Server::warnJoin(client.getSocketFD(), 442, channel);
    std::string messageReady = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " PRIVMSG " + channel_it->first + " :" + messageToSend + "\r\n"; 
    std::vector<int> &chan_clients = channel_it->second.getClients();
    for(size_t i = 0; i < chan_clients.size(); ++i)
    {
        int fd = chan_clients[i];
        if (fd == client.getSocketFD())
            continue;
        Client &dest = server.getClients()[fd];
        dest.addTowBuffer(messageReady);
        server.enableWriteEvent(fd);
    }
}

void privmsg(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() < 1)
        throw Server::warnRunning(client.getSocketFD(), 411);
    if(command.params.size() < 2)
        throw Server::warnRunning(client.getSocketFD(), 412);
    std::string receiver = command.params[0];
    std::string messageToSend = command.params[1];
    std::string nickFirstChar = "[]{}^|_`-";
    std::string isServer = "#&";
    if(!receiver.empty() && (std::isalpha(receiver[0]) || nickFirstChar.find(receiver[0]) != std::string::npos))
        sendToUser(server, client, receiver, messageToSend);
    else if (!receiver.empty() && (isServer.find(receiver[0]) != std::string::npos))
        sendToChannel(server, client, receiver, messageToSend);
    else
        throw Server::warnJoin(client.getSocketFD(), 401, receiver);
}