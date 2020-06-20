#pragma once

#include <sstream>
#include <string>

#include "sleepy_discord/sleepy_discord.h"

#include "types/types.hpp"

namespace Shiba::Discord {

    class DiscordUser : public User {
        public:
            DiscordUser(SleepyDiscord::User user) {
                std::ostringstream oss;
                oss<<"<@"<<user.ID.number()<<'>';
                pingable = oss.str();

                oss.str("");
                oss<<"Discord_"<<user.ID.number();
                ident = oss.str();
            }

            ~DiscordUser() {

            }

            std::string GetUsername() const noexcept override {
                return user.username;
            }

            std::string GetPingableUsername() const noexcept override {
                return pingable;
            }
            
            std::string GetIdentifier() const noexcept override {
                return ident;
            }

            bool GetBot() const noexcept override {
                return user.bot;
            }
            

        private:
            SleepyDiscord::User user;
            std::string pingable;
            std::string ident;
    };
    
    class DiscordMessage : public Message {
        public:
            DiscordMessage(SleepyDiscord::Message message, SleepyDiscord::DiscordClient &client)
            : client(client)
            , message(message)
            , userobj(DiscordUser(message.author)) {

            }

            User &GetAuthor() override {
                return userobj;
            }

            const User &GetConstAuthor() const noexcept override {
                return userobj;
            }

            std::string GetSourceName() const noexcept override {
                return "FIXME";
            }

            std::string GetFullSourceName() const noexcept override {
                return "FIXME";
            }

            std::string GetSourceIdentifier() const noexcept override {
                return std::string("Discord_") + message.channelID.string();
            }
            
            std::string GetContent() const noexcept override {
                return message.content;
            }

            void ReturnToSender(std::string content) override {
                bool sent = false;
                while (!sent) {
                    try {
                        client.sendMessage(message.channelID, content);
                        sent = true;
                    } catch (...) {}
                }
            }

        private:
            SleepyDiscord::DiscordClient &client;
            SleepyDiscord::Message message;
            DiscordUser userobj;
    };

}