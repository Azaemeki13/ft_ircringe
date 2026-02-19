/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 12:41:20 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/02 11:16:27 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Parameters: <username> <hostname> <servername> <realname>
/*it should probably skip the hostname and servername because we dont need server to server communication.
also it already knows it's own name (irserv).
i should only keep letters, digits, dashes, underscores for the username to avoid crashes. no spaces.
is WHOIS supposed to be supported?
realname can be empty. if so, it should probably set as "Anonymous" or something. that way WHOIS will display it like this.
*/
#include "Commands.hpp"

bool isValidUser(const std::string &user)
{
    if(user.empty())
        return(false);
    
    size_t i = 0;
    while(i < user.size())
    {
        if(user[i] == ' ' || user[i] == '\a' || user[i] == '\0' || user[i] == '\n' || user[i] == '\r' || user[i] == ':' || user[i] == '@')
            return(false);
        i++;
    }
    std::string lowerUser = user;
    std::transform(lowerUser.begin(), lowerUser.end(), lowerUser.begin(), ::tolower);
    if(lowerUser.find("admin") != std::string::npos)
        return(false);
    return(true);
}

bool isValidReal(const std::string &real)
{
    if(real.empty())
        return(true);
    
    size_t i = 0;
    while(i < real.size())
    {
        if(real[i] == '\a' || real[i] == '\0' || real[i] == '\n' || real[i] == '\r')
            return(false);
        i++;
    }
    return(true);
}

void user(Server &server, Client &client, const Commands &command)
{
    if(!client.getUserName().empty())
        throw Server::warnRunning(client.getSocketFD(), 462);
    
    if(command.params.size() != 4)
        throw Server::warnRunning(client.getSocketFD(), 461);
    std::string username = command.params[0];
    std::string realname = command.params[3];
    if(!isValidUser(username))
        throw Server::warnRunning(client.getSocketFD(), 432);
    if(!isValidReal(realname))
        throw Server::warnRunning(client.getSocketFD(), 432);
    
    client.setUserName(username);
    if(realname.empty())
        client.setRealName("Anonymous");
    else
        client.setRealName(realname);
    std::string nickname = client.getNickName();
    if(nickname.empty())
        nickname = "*";
    std::string welcome = ":server 001 " + nickname + " :Welcome to the IRC Network " + nickname + "!~" + username + "@localhost\r\n";
    client.addTowBuffer(welcome);
    server.enableWriteEvent(client.getSocketFD());
    std::string yourhost = ":server 002 " + nickname + " :Your host is server, running version 1.0\r\n";
    client.addTowBuffer(yourhost);
    server.enableWriteEvent(client.getSocketFD());
    std::string created = ":server 003 " + nickname + " :This server was created today\r\n";
    client.addTowBuffer(created);
    server.enableWriteEvent(client.getSocketFD());
    std::string myinfo = ":server 004 " + nickname + " server 1.0 o o\r\n";
    client.addTowBuffer(myinfo);
    server.enableWriteEvent(client.getSocketFD());
}
