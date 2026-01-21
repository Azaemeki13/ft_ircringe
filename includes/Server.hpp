/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:48:46 by cauffret          #+#    #+#             */
/*   Updated: 2026/01/21 14:01:10 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
    #define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <csignal>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <stdexcept>
#include <exception>
#include <sstream>
#define PORT 9090
#define MAX_EVENTS 10

class Server
{
    private :
    // data && variables
        int listener; // doorman
        int port; // adress of the door
        std::string password; // server password
        struct sockaddr_in6 serv_addr;
        std::map<int, Client> clients; // to store new connections
        std::map<std::string, Channel> channels;
        int epfd; // fd for my epoll
        struct epoll_event events[MAX_EVENTS]; // epoll array for all events
        struct epoll_event fd_ev; // other epoll array to add and modify fd's 

    // functions
        void initServ(int port);
        void handleNewConnection();
        void handleClientMessage(int fd);
        
    public : 
    // Allocs
        Server();
        Server(int port);
        ~Server();
        
    // loop xdxd
        void run(); 

    // error handling
        void sendError(Client &client, const std::string &code, const std::string &message);
    
    // getters
        std::string getPassword() const;
        void setPassword(const std::string &pwd);

    // exceptions
        class warnRunning : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };

};


#endif