/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:32:45 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/02 14:18:48 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

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
    std::map<int, Client>::iterator it = clients.begin();
    
    while(it != clients.end())
    {
        if(it->second.getNickName() == nick)
            return(true);
        ++it;
    }
    return(false);
}

/*NICK command is used to give user a nickname or change the existing one.*/

void nick(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() < 1)
        throw Server::warnRunning(client.getSocketFD(), 461);
    std::string newNick = command.params[0];
    if(newNick.empty())
        throw Server::warnRunning(client.getSocketFD(), 431);
    if(!isValidNick(newNick))
        throw Server::warnRunning(client.getSocketFD(), 432);
    if(isNickInUse(server, newNick))
        throw Server::warnRunning(client.getSocketFD(), 433);
    std::string oldNick = client.getNickName();
    client.setNickName(newNick);
    if (!client.getIsAuthorized()) 
        return;
    std::string response = ":" + oldNick + "!~" + client.getUserName() + "@" + client.getHostName() + " NICK " + newNick + "\r\n";
    std::set<int> neighbors; 
    std::map<std::string, Channel> &channels = server.getChannels();
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->second.isInChannel(client.getSocketFD()))
        {
            std::vector<int> &members = it->second.getClients();
            for (size_t i = 0; i < members.size(); i++)
            {
                if (members[i] != client.getSocketFD())
                    neighbors.insert(members[i]);
            }
        }
    }
    for (std::set<int>::iterator n_it = neighbors.begin(); n_it != neighbors.end(); ++n_it)
    {
        Client *neighbor = server.getClientPoint(*n_it);
        if (neighbor)
        {
            neighbor->addTowBuffer(response);
            server.enableWriteEvent(neighbor->getSocketFD());
        }
    }
    client.addTowBuffer(response);
    server.enableWriteEvent(client.getSocketFD());
}