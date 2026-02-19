/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 14:49:46 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/19 14:52:49 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void list(Server &server, Client &client, const Commands &command)
{
    (void)command;
    std::string listMsg = ":server 321 " + client.getNickName() + " Channel :Users Name\r\n";
    for (std::map<std::string, Channel>::iterator it = server.getChannels().begin(); it != server.getChannels().end(); ++it)
    {
        std::stringstream ss;
        ss << it->second.getClients().size();
        listMsg += ":server 322 " + client.getNickName() + " " + it->second.getChannelName() + " " + ss.str() + " :" + it->second.getTopic() + "\r\n";
    }
    listMsg += ":server 323 " + client.getNickName() + " :End of /LIST\r\n";
    client.addTowBuffer(listMsg);
    server.enableWriteEvent(client.getSocketFD());
    return; 
}