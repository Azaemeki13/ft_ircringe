/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 09:33:36 by cauffret          #+#    #+#             */
/*   Updated: 2026/02/04 17:36:25 by chsauvag         ###   ########.fr       */
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
        throw Server::warnJoin(client_fd, 461, "");
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
            throw Server::warnJoin(client_fd, 479, name);
    }
    return(result);
}

void join(Server &server, Client &client, const Commands &command)
{
    std::map<std::string, std::string> server_list = parseJoin(command, client.getSocketFD());
    std::map<std::string, std::string>::iterator pr_it = server_list.begin();
    
    for (; pr_it != server_list.end(); ++pr_it)
    {
        std::map<std::string, Channel>::iterator servit = server.getChannels().find(pr_it->first);
        bool isNewChannel = 0;
        if (servit != server.getChannels().end()) //if channel exists check pw
        {
            if (servit->second.getKey() != pr_it->second)
                throw Server::warnJoin(client.getSocketFD(), 475, pr_it->first);
            if (servit->second.isInviteOnly())
                throw Server::warnJoin(client.getSocketFD(), 473, pr_it->first);
            
        }
        else //if it doesn't exist, create it
        {
            server.getChannels()[pr_it->first] = Channel(pr_it->first, pr_it->second);
            servit = server.getChannels().find(pr_it->first);
            isNewChannel = 1;
        }
        if (isNewChannel)
            servit->second.addOperator(&client);
        if (servit->second.isInChannel(client.getSocketFD()))
            continue;
        servit->second.getClients().push_back(client.getSocketFD());
        std::string joinMsg = ":" + client.getNickName() + "!~" + client.getUserName() + "@" +
                              client.getHostName() + " JOIN :" + pr_it->first + "\r\n"; //broadcast
        servit->second.broadcastMessage(&client, joinMsg);
        std::string joinMsgSelf = ":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getHostName() + " JOIN " + pr_it->first + "\r\n";
        send(client.getSocketFD(), joinMsgSelf.c_str(), joinMsgSelf.length(), 0);
        std::string namesReply = ":server 353 " + client.getNickName() + " = " + pr_it->first + " :" + client.getNickName() + "\r\n";
        send(client.getSocketFD(), namesReply.c_str(), namesReply.length(), 0);
        std::string endOfNames = ":server 366 " + client.getNickName() + " " + pr_it->first + " :End of /NAMES list\r\n";
        send(client.getSocketFD(), endOfNames.c_str(), endOfNames.length(), 0);
        if (!servit->second.getTopic().empty())
        {
            std::string topicMsg = ":server 332 " + client.getNickName() + " " + pr_it->first + " :" + servit->second.getTopic() + "\r\n";
            send(client.getSocketFD(), topicMsg.c_str(), topicMsg.length(), 0);
        }
    }
}