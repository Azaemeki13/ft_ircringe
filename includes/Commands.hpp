/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:54:30 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/04 16:35:07 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Client.hpp"
#include "Parser.hpp"
#include "Server.hpp"
#include "Channel.hpp"

class Server;

//-----authentification flow
void pass(Server &server, Client &client, const Commands &command);
void nick(Server &server, Client &client, const Commands &command);
void user(Client &client, const Commands &command);
void pong(Client &client, const Commands &command);

//-----messaging
void privmsg(Server &server, Client &client, const Commands &command);

//-----channel management
void join(Server &server, Client &client, const Commands &command);
void topic(Server &server, Client &client, const Commands &command);
void kick(Server &server, Client &client, const Commands &command);
void mode(Server &server, Client &client, const Commands &command);

// utils
std::vector<std::string> split(const std::string &str, char delimiter);


#endif