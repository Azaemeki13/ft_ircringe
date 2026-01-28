/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 10:42:59 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/28 13:41:01 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*Command: PRIVMSG
   Parameters: <receiver>{,<receiver>} <text to be sent>*/

//bool isValidReciever(Server &server, const std::string &receiver)

Client* Server::getClientByNickname(const std::string& nickname)
{
    std::map<int, Client>& clients = const_cast<std::map<int, Client>&>(getClients());
    std::map<int, Client>::iterator it = clients.begin();

    while(it != clients.end())
    {
        if(it->second.getNickName() == nickname)
            return &(it->second);
        it++;
    }
    return NULL;
}

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
    Client *recipient = server.getClientByNickname(receiver);
    if(!recipient)
    {
        throw Server::warnRunning(client.getSocketFD(), 401);
        return ;
    }
    std::string messageReady = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " PRIVMSG " + receiver + " :" + messageToSend + "\r\n";
    ::send(recipient->getSocketFD(), messageReady.c_str(), messageReady.length(), 0);
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
    if(!receiver.empty() && (std::isalpha(receiver[0]) || nickFirstChar.find(receiver[0]) != std::string::npos))
        sendToUser(server, client, receiver, messageToSend);
}