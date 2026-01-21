/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 16:44:51 by cauffret          #+#    #+#             */
/*   Updated: 2026/01/21 13:57:16 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main (int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>." << std::endl;
        return(1);
    }
    int port = std::atoi(argv[1]);
    std::string password = argv[2];
    try
    {
        Server serv(port);
        serv.setPassword(password);
        serv.run();
    }
    catch (std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return(1);
    }
}