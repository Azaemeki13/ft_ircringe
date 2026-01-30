/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 09:33:36 by cauffret          #+#    #+#             */
/*   Updated: 2026/01/28 14:20:02 by cauffret         ###   ########.fr       */
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
        throw Server::warnJoin(461, client_fd, "");
        //return server.sendError(client, "461", "JOIN :Not enough parameters");
    std::vector<std::string> channels_n = split(command.params[0], ',');
    std::vector<std::string> channels_p;
    if (command.params.size() == 2 )
    {
        channels_p = split(command.params[1], ',');
    }
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
        else 
            throw Server::warnJoin(479, client_fd, name);
    }
    return(result);
}

void join(Server &server, Client &client, const Commands &command)
{
    std::map<std::string, std::string> server_list = parseJoin(command, client.getSocketFD());
    std::map<std::string, std::string>::iterator pr_it = server_list.begin();
    std::map<std::string, Channel>::iterator servit;
    for (; pr_it != server_list.end(); ++pr_it)
    {
        servit = server.getChannels().find(pr_it->first);
        // case 1 finds the channel
        if (servit != server.getChannels().end()) // conditions en mode invite only etc a rajouter ici
        {
            if (servit->second.getKey() == pr_it->second) // pw matching
            {
                servit->second.getClients().push_back(client.getSocketFD());
                std::stringstream ss;
                ss << "Succesfully joined the channel " << pr_it->first;
                std::string successMessage = ss.str();
                send(client.getSocketFD(), successMessage.c_str(), successMessage.length(), 0);
            }
            else 
                throw Server::warnJoin(475, client.getSocketFD(), pr_it->first);
        }
        else
        {
            // step 1 create channel
            server.getChannels()[pr_it->first] = Channel(pr_it->first, pr_it->second);
            // step 2 insert the client as user AND admin 
            // ADD ADMIN TBD 
            servit = server.getChannels().find(pr_it->first);
            servit->second.getClients().push_back(client.getSocketFD());
            std::stringstream ss;
            ss << "Succesfully created the channel " << pr_it->first;
            std::string successMessage = ss.str();
            send(client.getSocketFD(), successMessage.c_str(), successMessage.length(), 0);
        }
    }
}