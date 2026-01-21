/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 16:44:51 by cauffret          #+#    #+#             */
/*   Updated: 2026/01/21 13:59:28 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main (int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./" << argv[0] << " <port> <password>." << std::endl;
        return(1);
    }
    int port = std::atoi(argv[1]);
    std::string password = argv[2];
    try
    {
        Server serv(port);
        serv.setPassword(password);
        
        // Test: Create a dummy client and send error
        Client testClient(0, "127.0.0.1");
        serv.sendError(testClient, "461", "Not enough parameters");
        std::cout << "Test: Sent error message" << std::endl;
        std::cout << "Test: Password set to: " << serv.getPassword() << std::endl;
        
        serv.run();
    }
    catch (std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return(1);
    }
}