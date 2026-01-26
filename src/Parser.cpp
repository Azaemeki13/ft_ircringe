/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 14:08:04 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/26 14:00:58 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Parser.hpp"
#include <iostream>

std::string trim(const std::string &line)
{
    size_t start = line.find_first_not_of(" \t\n\r");
    size_t end = line.find_last_not_of(" \t\n\r");

    if(start == std::string::npos) //empty command
        return("");
    return(line.substr(start, end - start + 1));
}
Commands::Commands(const std::string &message, int fd) socketFD(fd)
{
    std::string trimmedMessage = trim(message);
    std::stringstream ss(trimmedMessage);
    std::string token;
    if(ss >> token && token[0] == ':') //prefix
        prefix = token.substr(1);
    else if(!token.empty())
    {
        command = token;
        token.clear();
    }
    if(command.empty() && ss >> token) //command
        command = token;
    while(ss >> token) //params
    {
        if(token[0] == ':')
        {
            std::string trailing = token.substr(1);
            std::string rest;
            if(getline(ss, rest))
                trailing += rest;
            params.push_back(trim(trailing));
            break ;
        }
        else
            params.push_back(token);
    }   
}