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

volatile sig_atomic_t server_running = 1;

void signal_handler(int)
{
    std::cout << "\nSignal received ! shutting down" << std::endl;
    server_running = 0;
    return;
}

int main (int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>." << std::endl;
        return(1);
    }
    int port = std::atoi(argv[1]);
    std::string password = argv[2];
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    bool start = 1;
    try
    {
        Server serv(port, password);
        while (server_running)
        {
            if (start == 1)
            {
                std::cout << "Server is running (Epoll mode) ... " << std::endl;
                start = !start;
            }
            serv.run();
        }
        return 0;
    }
    catch (std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return(1);
    }
}