/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 13:06:12 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/27 15:41:33 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp"

/* The PASS command is used to set a 'connection password'.  The
   optional password can and MUST be set before any attempt to register
   the connection is made.  Currently this requires that user send a
   PASS command before sending the NICK/USER combination.*/

void pass(Server &server, Client &client, const Commands &command)
{
    if(command.params.size() < 1)
        throw Server::warnRunning(client.getSocketFD(), 461);
    if(client.getIsAuthorized() == true)
        throw Server::warnRunning(client.getSocketFD(), 462);
    std::string password = command.params[0];
    if(password != server.getPassword())
        throw Server::warnRunning(client.getSocketFD(), 464);
    bool &authorisations = client.getIsAuthorized();
    authorisations = true;
}