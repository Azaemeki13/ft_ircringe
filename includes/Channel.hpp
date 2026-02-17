/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:52:49 by chsauvag          #+#    #+#             */
/*   Updated: 2026/02/06 13:45:43 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
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

class Server;

class Channel 
{
    private:
        //-----basic channel info
        std::string channelName;
        std::string topic;
        
        //-----client vector
        std::vector<int> clients;
        std::vector<int> operators;
        
        //-----modes
        bool inviteOnly; //-i
        bool topicProtected; //-t
        std::string key; //-k
        int userLimit; //-l
        
    public:
        //-----constructors
        Channel(const std::string &channelName);
        Channel(const std::string &channelName, const std::string &password);
        Channel();
        ~Channel();
        
        //-----getters
        const std::string &getChannelName() const;
        const std::string &getTopic() const;
        std::vector<int> &getClients();
        std::vector<int> &getOperators();
        const std::vector<int> &getOperators() const;

        bool isInviteOnly() const;
        bool isTopicProtected() const;
        const std::string getKey() const;
        int getUserLimit() const;

        //-----client management
        void addClient(Client* client);
        void removeClient(Client* client);
        void removeClient(int client_fd);
        bool isInChannel(int client) const;

        //-----operator management 
        void addOperator(Client* client);
        void removeOperator(Client* client);
        void removeOperator(int client);
        bool isOperator(const int client) const;

        //-----broadcast messaging
        void broadcastMessage(Server &server, int sender_fd, const std::string &msg);
        
        //-----setters (mode options)
        void setInviteOnly(bool state);
        void setTopicProtected(bool state);
        void setKey(const std::string &key); //string empty = remove key
        void setUserLimit(int limit);

        //-----setters (channel info)
        void setChannelName(std::string &name);
        void setTopic(const std::string &topic);

        std::vector<int> invitedUsers;
        void addInvitedUser(int client_fd);
        void removeInvitedUser(int client_fd);
        bool isInvited(int client_fd) const;
};

#endif