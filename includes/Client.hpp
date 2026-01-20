/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:50:37 by chsauvag          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2026/01/20 18:08:44 by chsauvag         ###   ########.fr       */
=======
/*   Updated: 2026/01/20 18:30:39 by cauffret         ###   ########.fr       */
>>>>>>> fb9b16e (Charles 01-20 1832 added map to include Client, now need to fix Client shell)
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#define NEED_PASS 0
#define NEED_NICK 0
#define NEED_USER 0
#define REGISTERED 0

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

class Client 
{
        public:
    //-----CONSTRUCTOR
        Client(int fd, const std::string &host);
        Client();
        ~Client();
        
    //-----NETWORK IDENTITY
        int socketFD; //used by poll(), should be set by server during client creation
        std::string getSocketFD();
        
        time_t getConnexionTime() const;
        time_t connexionTime;
        int authState;

    //-----IRC identity
        std::string nickName;
        std::string userName;
        std::string realName;
        std::string hostName; //for USER prefix, should be set by server during client creation

        std::string getNickName() const;
        std::string getUserName() const;
        std::string getRealName() const;
        std::string getHostName() const;

        void setNickName(const std::string &nick);
        void setUserName(const std::string &user);
        void setRealName(const std::string &real);
        void setHostName(const std::string &host);
    
    //-----active session info        
        std::vector<std::string> channels;
        std::vector<std::string> joinedChannels;
        std::vector<std::string> extractedCommands; //without \r\n i guess lolz

        void leaveChannel(const std::string &channelName);
        void joinChannel(const std::string &channelName);
        void isInChannel(const std::string &channelName);
    
    //-----state
        /*void setAuthState();*/      
};

#endif