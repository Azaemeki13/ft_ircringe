/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 09:33:36 by cauffret          #+#    #+#             */
/*   Updated: 2026/01/27 15:40:28 by chsauvag         ###   ########.fr       */
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
    std::stringstream ss(str);
    while(std::getline(ss, token, delimiter))
    {
        if(!token.empty())
            tokens.push_back(token);
    }
    return(tokens);
}

std::map<std::string, std::string> parseJoin(const Commands &command, int client_fd)
{
    std::map<std::string, std::string> result;
    if(command.params.empty())
        throw Server::warnRunning(461, client_fd);
        //return server.sendError(client, "461", "JOIN :Not enough parameters");
    std::vector<std::string> channels_n = split(command.params[0], ',');
    if (command.params.size() == 2 )
    {
        std::vector<std::string> channels_p = split(command.params[1], ',');
        for (size_t i = 0; i < channels_n.size(); ++i)
        {
            std::string name = channels_n[i];
            if ((name[0] == '&') || (name[0] == '#'))
            {
                if (i < channels_p.size())
                    result.insert(std::make_pair(channels_n[i], channels_p[i]));
                else
                    result.insert(std::make_pair(channels_n[i], ""));
            }
            
        }
    }
    return(result);
}

void join(Server &server, Client &client, const Commands &command)
{
    (void)server;
    (void)client;
    (void)command;
}