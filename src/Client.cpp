/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:50:42 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/21 10:08:19 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, const std::string &host) : 
    socketFD(fd), _connexionTime(time(NULL)),  authState(NEED_PASS), 
    nickName(""),  userName(""), realName(""), hostName(host)
{
    joinedChannels.clear(); //starts empty
}

Client::~Client()
{
    std::cout << "Destructor called on client." << std::endl;
}



