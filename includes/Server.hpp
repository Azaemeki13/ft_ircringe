/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:48:46 by cauffret          #+#    #+#             */
/*   Updated: 2026/02/06 11:02:41 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
    #define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include "Commands.hpp"
#include <algorithm>
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
        void processCommand(Client &client, const std::string &message);
        
    public : 
    // Allocs
        Server();
        Server(int port);
        ~Server();
        
    // loop xdxd
        void run(); 

    // error handling
        void sendError(Client &client, const std::string &code, const std::string &message);
        std::string getErrorMessage(int code, const Commands &cmd) const;

    
    // getters
        std::string getPassword() const;
        void setPassword(const std::string &pwd);
        const std::map<int, Client>& getClients() const;
        std::map<int, Client>& getClients();
        std::map<std::string, Channel>& getChannels();
        int getClientByNickname(const std::string& nickname);
        const std::string getClientByFD(int fd); 

    // exceptions
        class warnRunning : public std::exception
        {
            private:
                int client_fd;
                int errorCode;
                std::string message;
            public:
                warnRunning(int fd, int code);
                int getFD() const ;
                int getErrorCode() const;
                virtual const char *what() const throw();
                virtual ~warnRunning() throw();
        };
        class warnJoin : public std::exception
        {
            private:
                int client_fd;
                int errorCode;
                std::string channel;
            public:
                warnJoin(int fd, int code, std::string channel_name);
                int getFD() const ;
                int getErrorCode() const;
                std::string getChannelName() const;
                virtual const char *what() const throw();
                virtual ~warnJoin() throw();
        };
        void handleError(const warnRunning &e, Client &client, const Commands &cmd);
};


#endif