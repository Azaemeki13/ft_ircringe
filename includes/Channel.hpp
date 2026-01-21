/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:52:49 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/21 11:07:53 by chsauvag         ###   ########.fr       */
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
        //-----constructors
        Channel(const std::string& channelName);
        Channel();
        ~Channel();
        
        //-----getters
        const std::string& getChannelName() const;
        const std::string& getTopic() const;
        const std::vector<*Client>& getClients() const;
        const std::vector<*Client>& getOperators() const;

        bool isInviteOnly();
        bool isTopicProtected();
        const std::string getKey();
        int getUserLimit();

        //-----client management
        void addClient(Client* client);
        void removeClient(Client* client);
        void isInChannel(Client* client);

        //-----operator management 
        void addOperator(Client* operator);
        void removeOperator(Client* operator);
        void isOperator(Client* operator);

        //-----broadcast messaging
        void broadcastMessage(Client* fromWho, const std::string& msg)
        
        //-----setters (mode options)
        void setInviteOnly(bool state);
        void setTopicProtected(bool state);
        void setKey(std::string& key); //string empty = remove key
        void setUserLimit(int limit);

        //-----setters (channel info)
        void setChannelName(std::string& name);
        void setTopic(std::string& topic);

        #endif
        
}