#pragma once

#include <sstream>
#include <string>

#include "sleepy_discord/sleepy_discord.h"

#include "shiba/types/user.hpp"
#include "shiba/types/message.hpp"

namespace Shiba::Discord {

    class DiscordUser : public User {
        public:
            DiscordUser(SleepyDiscord::User user);
            ~DiscordUser() override;

            std::string GetUsername() const override;
            std::string GetPingableUsername() const override;
            std::string GetIdentifier() const override;
            bool GetBot() const override;

        private:
            SleepyDiscord::User user;
            std::string pingable;
            std::string ident;
    };
    
    class DiscordMessage : public Message {
        public:
            DiscordMessage(SleepyDiscord::Message message, SleepyDiscord::DiscordClient &client);
            ~DiscordMessage() override;

            User &GetAuthor() override;
            const User &GetConstAuthor() const override;
            std::string GetSourceName() const override;
            std::string GetFullSourceName() const override;
            std::string GetSourceIdentifier() const override;
            std::string GetContent() const override;
            void ReturnToSender(std::string content) override;

        private:
            SleepyDiscord::DiscordClient &client;
            SleepyDiscord::Message message;
            DiscordUser userobj;
    };

}