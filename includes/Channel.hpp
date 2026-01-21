/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:52:49 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/21 14:00:15 by chsauvag         ###   ########.fr       */
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
        Channel();
        ~Channel();
        
        //-----getters
        const std::string &getChannelName() const;
        const std::string &getTopic() const;
        const std::vector<Client*> &getClients() const;
        const std::vector<Client*> &getOperators() const;

        bool isInviteOnly() const;
        bool isTopicProtected() const;
        const std::string getKey() const;
        int getUserLimit() const;

        //-----client management
        void addClient(Client* client);
        void removeClient(Client* client);
        void isInChannel(Client* client) const;

        //-----operator management 
        void addOperator(Client* client);
        void removeOperator(Client* client);
        void isOperator(Client* client) const;

        //-----broadcast messaging
        void broadcastMessage(Client* fromWho, const std::string &msg);
        
        //-----setters (mode options)
        void setInviteOnly(bool state);
        void setTopicProtected(bool state);
        void setKey(std::string &key); //string empty = remove key
        void setUserLimit(int limit);

        //-----setters (channel info)
        void setChannelName(std::string &name);
        void setTopic(std::string &topic);
};

#endif