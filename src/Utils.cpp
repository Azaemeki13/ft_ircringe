/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:01:15 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/21 16:07:08 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"

void handleExceptions(const Server::warnJoin &e, Client &client, Server &server)
{
    std::stringstream codeStream;
    codeStream << e.getErrorCode();
    std::string errorCode = codeStream.str();
    switch (e.getErrorCode())
    {
        case 401: // ERR_NOSUCHNICK
        {
            std::string msg = e.getChannelName() + " :No such nickname !";
            server.sendError(client, errorCode, msg);
            break;
        }

        case 403: // ERR_NOSUCHCHANNEL
        {
            std::string msg = "KICK " + e.getChannelName() + " :No such channel !";
            server.sendError(client, errorCode, msg);
            break;
        }

        case 441: // ERR_USERNOTINCHANNEL
        {
            std::string msg = "KICK " + e.getChannelName() + " :User is not on the channel !";
            server.sendError(client, errorCode, msg);
            break;
        }

        case 442: // ERR_NOTONCHANNEL
        {
            std::string msg = e.getChannelName() + " :Target user of the command is not on the channel !";
            server.sendError(client, errorCode, msg);
            break;
        }

        case 461: // ERR_NEEDMOREPARAMS
        {
            if (e.getChannelName() == "KICK ")
            {
                server.sendError(client, errorCode, "KICK :Not enough parameters.");
                break;
            }
            else
            {
                server.sendError(client, errorCode, "JOIN :Not enough parameters");
                break;
            }
        }
  
        case 473: // ERR_INVITEONLYCHAN
        {
            std::string msg = "JOIN " + e.getChannelName() + " :Cannot join channel (+i) - Invite only";
            server.sendError(client, errorCode, msg);
            break;
        }

        case 475: // ERR_BADCHANNELKEY
        {
            std::string msg = "JOIN " + e.getChannelName() + " :Cannot join channel (+k) - Bad Key";
            server.sendError(client, errorCode, msg);
            break;
        }

        case 482: // ERR_CHANOPRIVSNEEDED
        {
            std::string msg = "KICK " + e.getChannelName() + " :Not enough privileges to perform this action !";
            server.sendError(client, errorCode, msg);
            break;
        }

        case 479: // ERR_BADCHANNAME (Non-standard, but often used for illegal chars)
        {
            server.sendError(client, errorCode, "JOIN :Erroneous Channel Name"); 
            break;
        }

        default:
        {
            server.sendError(client, "400", ":Unknown Error during Join");
            break;
        }

    }
}
