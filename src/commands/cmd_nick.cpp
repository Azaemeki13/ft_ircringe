/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:32:45 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/27 11:20:15 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp"
#include "Utils.hpp"

/*Common Nickname Rules & Invalid Characters:
Allowed Characters: Letters (A-Z, a-z), numbers (0-9), hyphens (-), and specific symbols often include []{}^|_`. ->ok
Disallowed Characters: Spaces, NUL (\x00), BELL (\x07), Line Feed (\x0A), Carriage Return (\x0D), and the colon : are typically forbidden. ->ok
First Character: Must be a letter or one of the allowed special characters; names starting with numbers (like 4chan) or certain symbols (like ///) can be invalid. ->ok
Length: While RFCs suggest a limit (e.g., 9 characters), many networks allow longer nicks (e.g., 32 characters). ->ok
Uniqueness: Nicknames must generally be unique on the network; using someone else's (or a reserved service name) can lead to disconnects. 
Why Nicknames Become Invalid:
Protocol Violations: Using forbidden characters breaks the IRC protocol. ->ok
Network Limits: Exceeding the network's maximum nickname length. ->ok
Security/Abuse: Using names reserved for network staff or common names (like "Admin"). ->ok*/

bool isValidNick(const std::string &nick)
{
    if(nick.empty())
        return(false);
    if(nick.size() > 9)
        return(false);
    
    size_t i = 0;
    while(i < nick.size())
    {
        if(nick[i] == ' ' || nick[i] == '\a' || nick[i] == '\0' || nick[i] == '\n' || nick[i] == '\r' || nick[i] == ':')
            return(false);
        i++;
    }
    const std::string specials = "[]{}^|_`-";
    if(!std::isalpha(nick[0]) && specials.find(nick[0]) == std::string::npos)
        return(false);
    std::string lowerNick = nick;
    std::transform(lowerNick.begin(), lowerNick.end(), lowerNick.begin(), ::tolower);
    if(lowerNick.find("admin") != std::string::npos)
        return(false);
    return(true);
}

bool isNickInUse(Server &server, const std::string &nick)
{
    std::map<int, Client> clients = server.getClients();
    while(clients.size())
    {
        std::map<int, Client>::iterator it = clients.begin();
        if(it->second.getNickName() == nick)
            return(true);
    }
    return(false);
}

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