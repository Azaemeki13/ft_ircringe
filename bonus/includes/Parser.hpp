/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 14:08:00 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/27 13:20:34 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <sstream>

class Commands
{
        private:

        public:
            Commands(const std::string &message, int fd);
            std::string prefix;
            std::string command;
            std::vector<std::string> params;
            int client_fd;
};

Commands parseCommands(const std::string &line);

#endif