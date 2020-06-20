#pragma once

#include <string>

#include "shiba/types/user.hpp"

namespace Shiba {

    //An abstract class that contains information about a received message
    //All messages are supposed to be passed off as a Message * to the bot core.
    class Message {
        public:
            virtual ~Message() = default;

            //Returns a User object for whoever sent the message
            virtual User &GetAuthor() = 0;
            virtual const User &GetConstAuthor() const = 0;

            //Returns a readable string of where the message was sent from
            //IRC: ##hellomouse
            //Discord: #general
            virtual std::string GetSourceName() const = 0;

            //Returns a readable string of where the message was sent from
            //More descriptive than GetSourceName()
            //IRC: freenode ##hellomouse
            //Discord: Hellomouse #general
            virtual std::string GetFullSourceName() const = 0;

            //Returns an identifier of where the message was sent from
            //IRC: IRC_freenode_##hellomouse
            //Discord: Discord_271781178296500235
            virtual std::string GetSourceIdentifier() const = 0;

            //Returns an identifier of where the message generally was from
            //IRC: IRC_freenode
            //Discord: Discord_271781178296500235
            //virtual std::string GetGeneralSourceIdentifier() const = 0;

            //Returns the text contents of the message
            //For advanced text features for a platform, cast this Message object to a more specific one such as DiscordMessage
            virtual std::string GetContent() const = 0;

            //Returns a reply to where the message was received from
            virtual void ReturnToSender(std::string content) = 0;
    };

}