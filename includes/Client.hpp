/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:50:37 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/27 13:24:43 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#define NEED_PASS 0
#define NEED_NICK 0
#define NEED_USER 0
#define REGISTERED 0

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
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
#include <ctime>

class Client 
{
        private:
            int socketFD;
            time_t _connexionTime;
            bool isAuthorized;
            int authState;

            std::string nickName;
            std::string userName;
            std::string realName;
            std::string hostName;
            std::string buffer;

            std::vector<std::string> channels;
            std::vector<std::string> joinedChannels;
            std::vector<std::string> extractedCommands;
        public:
    //----- mini OCF
        Client(int fd, const std::string &host);
        Client();
        ~Client();
        
    //DSL CHLO JE MY RETROUVE PAS 
    //-----setters
        void setNickName(const std::string &nick);
        void setUserName(const std::string &user);
        void setRealName(const std::string &real);
        void setHostName(const std::string &host);

    //-----getters 
        int getSocketFD() const;
        time_t getConnexionTime() const;
        bool &getIsAuthorized();
        std::string &getNickName();
        std::string &getUserName();
        std::string &getRealName();
        std::string &getHostName();
        std::string &getBuffer();

    //-----Functions 
        void leaveChannel(const std::string &channelName);
        void joinChannel(const std::string &channelName);
        void isInChannel(const std::string &channelName);
    
    //-----state
        /*void setAuthState();*/

    //-----buffer 
            //for USER prefix, should be set by server during client creation
            //-----active session info        
            //without \r\n i guess lolz
};

#endif