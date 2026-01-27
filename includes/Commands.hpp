/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:54:30 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/27 13:07:32 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Client.hpp"
#include "Parser.hpp" //to get the cmd struct, TMTC

class Server;

//-----authentification flow
void pass(Server &server, Client &client, const Commands &command);
void nick(Server &server, Client &client, const Commands &command);
void user(Server &server, Client &client, const Commands &command);
void join(Server &server, Client &client, const Commands &command);

#endif