/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 09:33:36 by cauffret          #+#    #+#             */
/*   Updated: 2026/01/26 13:08:05 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
le token est forcement split au niveau du password

• copier de # a  ','  OU si pas de ',' # dans un vector et token apres jusqu'a virgule */

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream(str);
    while(std::getline(ss, token, delimiter))
    {
        if(!token.isempty())
            tokens.push_back(token);
    }
    return(tokens);
}

std::map<std::string, std::string> parseJoin(const Commands &command)
{
    std::map<std::string, std::string> result;
    if(commands.params.empty())
        throw warnRunning();
        //return server.sendError(client, "461", "JOIN :Not enough parameters");
    std::vector<std::string> channels_n = split(params[0], ',');
    std::vector<std::string> channels_p = split(params[1], ',');
    for (size_t i = 0; i < channels_n.size(); ++i)
    {
        size_t posh = channels[n].find('#');
        size_t posa = channels[n].find('&');
        if ((posa == std::string::npos) && (posh == std::string::npos))
    }

}

void join(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() > 1)
        return server.sendError(client, "461", "PASS :Not enough parameters");
    std::string channelName = &command.params[0];
    if (!command.params[1].empty())
        std::string password = &command.params[1];
    if(channelName.empty())
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