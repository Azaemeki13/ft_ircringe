/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:52:44 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/21 14:03:05 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string& channelName) : channelName(channelName), topic(""), inviteOnly(false),
    topicProtected(false), key(""), userLimit(0)
{
    clients.clear();
    operators.clear();
}

Channel::~Channel()
{
    std::cout << "Destructor called on channel." << std::endl;
}