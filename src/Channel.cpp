/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:52:44 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/28 13:56:15 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() : channelName(""), topic(""), inviteOnly(0),
        topicProtected(0), key(""), userLimit(0)
{
    clients.clear();
    operators.clear();
}

Channel::Channel(const std::string& channelName) : channelName(channelName), topic(""), inviteOnly(false),
    topicProtected(false), key(""), userLimit(0)
{
    clients.clear();
    operators.clear();
}

Channel::Channel(const std::string& channelName, const std::string &password) : channelName(channelName), topic(""), inviteOnly(false),
    topicProtected(false), key(password), userLimit(0)
{
    clients.clear();
    operators.clear();
}

Channel::~Channel()
{
    std::cout << "Destructor called on channel." << std::endl;
}

std::vector<int> &Channel::getClients()
{
    return(clients);
}
const std::string Channel::getKey() const
{
    return key;
}
