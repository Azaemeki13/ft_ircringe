/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:05:16 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/04 15:08:44 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void pong(Server &server, Client &client, const Commands &command)
{
    std::string pong;
    if (command.params.size() > 0)
        pong = "PONG server :" + command.params[0] + "\r\n";
    else
        pong = "PONG server\r\n";
    client.addTowBuffer(pong);
    server.enableWriteEvent(client.getSocketFD());
    return;
}