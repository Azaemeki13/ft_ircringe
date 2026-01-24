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

/*Common Nickname Rules & Invalid Characters:
Allowed Characters: Letters (A-Z, a-z), numbers (0-9), hyphens (-), and specific symbols often include []{}^|_`.
Disallowed Characters: Spaces, NUL (\x00), BELL (\x07), Line Feed (\x0A), Carriage Return (\x0D), and the colon : are typically forbidden.
First Character: Must be a letter or one of the allowed special characters; names starting with numbers (like 4chan) or certain symbols (like ///) can be invalid.
Length: While RFCs suggest a limit (e.g., 9 characters), many networks allow longer nicks (e.g., 32 characters).
Uniqueness: Nicknames must generally be unique on the network; using someone else's (or a reserved service name) can lead to disconnects. 
Why Nicknames Become Invalid:
Protocol Violations: Using forbidden characters breaks the IRC protocol.
Network Limits: Exceeding the network's maximum nickname length.
Security/Abuse: Using names reserved for network staff or common names (like "Admin").*/

bool isValidNick(const std::string &nick)
{
    if(nick.empty())
        return(false);
    if(nick.size() > 9)
        return(false);
    int i = 0;
    while(nick[i])
    {
        if(nick == " " || nick == "\a" || nick == "\0" || nick == "\a" || nick == "\n" || nick == "\r" || nick == ":")
            return(false);
    }
    const std::string specials = "[]{}^|_`-";
    if(!std::isalpha(nick[0]) && specials.find(nick[0]) == std::string::npos)
        return(false);
    return(true);
}

//bool isNickInUse(Server &server, const std::string &nick);

/*NICK command is used to give user a nickname or change the existing one.*/

void nick(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() < 1)
        return server.sendError(client, "461", "PASS :Not enough parameters");
    std::string newNick = command.params[0];
    if(newNick.empty())
        return server.sendError(client, "431", ":No nickname given");
    if(!isValidNick(newNick))
        return server.sendError(client, "432", newNick + " :Erroneous nickname");
    //if(isNickInUse(server, newNick))
        //return server.sendError(client, "433", newNick + " :Nickname is already in use");
    std::string oldNick = client.getNickName();
    client.setNickName(newNick);
    std::string response = ":" + oldNick + " NICK :" + newNick + "\r\n";
    send(client.socketFD, response.c_str(), response.length(), 0);
}