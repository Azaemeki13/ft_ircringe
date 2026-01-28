/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:54:30 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/28 09:53:17 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Client.hpp"
#include "Parser.hpp" //to get the cmd struct, TMTC
#include "Server.hpp"

class Server;

//-----authentification flow
void pass(Server &server, Client &client, const Commands &command);
void nick(Server &server, Client &client, const Commands &command);
void user(Client &client, const Commands &command);
void join(Server &server, Client &client, const Commands &command);

#endif