/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 13:06:12 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/21 13:55:35 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/* The PASS command is used to set a 'connection password'.  The
   optional password can and MUST be set before any attempt to register
   the connection is made.  Currently this requires that user send a
   PASS command before sending the NICK/USER combination.*/

void pass(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() < 1)
        return server.sendError(client, "461", "PASS :Not enough parameters");
    if(client.isAuthorized)
        return server.sendError(client, "462", ":Unauthorized command (already registered)");
    std::string password = command.params[0];
    if(password != server.getPassword())
        return server.sendError(client, "464", ":Password incorrect");
    client.isAuthorized = true;
}