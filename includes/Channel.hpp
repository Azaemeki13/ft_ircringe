/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:52:49 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/20 18:33:14 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class Channel 
{
    private:
        //-----basic channel info
        std::string channelName;
        std::string topic;
        
        //-----client vector
        std::vector<*Clients> clients;
        std::vector<*Clients> operators;
        
        //-----modes
        bool inviteOnly; //-i
        bool topicProtected; //-t
        std::string key; //-k
        int userLimit; //-l
        
    public:
        //-----
        const std::string& getChannelName() const;
        const std::string& getTopic() const;
        
}