/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:32:45 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/21 15:59:43 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp"
#include "Utils.hpp"

/*NICK command is used to give user a nickname or change the existing one.*/

/*void nick(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() < 1)
        return server.sendError(client, "461", "PASS :Not enough parameters");
    std::string newNick = command.params[0];
    if(newNick.empty())
        return server.sendError(client, "431", ":No nickname given");
    if(!isValidNick(newNick))
        return server.sendError(client, "432", newNick + " :Erroneous nickname");
    if(isNickInUse(server, newNick))
        return server.sendError(client, "433", newNick + " :Nickname is already in use");
    std::string oldNick = client.getNickName();
    client.setNickName(newNick);
    std::string response = ":" + oldNick + " NICK :" + newNick + "\r\n";
    send(client.socketFD, response.c_str(), response.length(), 0);
}*/