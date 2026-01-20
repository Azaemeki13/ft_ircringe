/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cauffret <cauffret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:50:42 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/20 17:11:17 by cauffret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp

Client::Client(int fd, const std::string &host) : 
    socketFD(fd), hostName(host), connexionTime(std::time(NULL)),
    nickName(""), userName(""), realName(""), authState(NEEDPASS)
{
    joinedChannels.clear(); //starts empty
}



