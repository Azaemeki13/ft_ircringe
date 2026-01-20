/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 14:08:04 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/20 17:55:52 by chsauvag         ###   ########.fr       */
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

Commands parseCommands(const std::string &line)
{
    Commands cmd;
    std::string trimmedLine = trim(line);
    std::stringstream ss(trimmedLine);
    std::string token;
    
    if(ss >> token && token[0] == ':') //prefix
        cmd.prefix = token.substr(1);
    else if(!token.empty())
    {
        cmd.command = token;
        token.clear();
    }
    if(cmd.command.empty() && ss >> token) //command
        cmd.command = token;
    while(ss >> token) //params
    {
        if(token[0] == ':')
        {
            std::string trailing = token.substr(1);
            std::string rest;
            if(getline(ss, rest))
                trailing += rest;
            cmd.params.push_back(trim(trailing));
            break ;
        }
        else
            cmd.params.push_back(token);
    }   
    return(cmd);
}