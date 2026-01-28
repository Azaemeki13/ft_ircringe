/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 10:42:59 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/28 13:07:47 by chsauvag         ###   ########.fr       */
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
                if(second->)
            }
            
        }
    }
}

void sendToChannel()
void sendToUser()
   
void privmsg(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() < 1)
        throw Server::warnRunning(client.getSocketFD(), 411);
    if(command.params.size() < 2)
        throw Server::warnRunning(client.getSocketFD(), 412);
    if() //#, &, +, ! == channel
        sendToChannel()
    else //user
        sendToUser()
    
}