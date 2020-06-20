#include "shiba/frontends/discordtypes.hpp"

namespace Shiba::Discord {

DiscordUser::DiscordUser(SleepyDiscord::User user) {
    std::ostringstream oss;
    oss<<"<@"<<user.ID.number()<<'>';
    pingable = oss.str();

    oss.str("");
    oss<<"Discord_"<<user.ID.number();
    ident = oss.str();
}

DiscordUser::~DiscordUser() {

}

std::string DiscordUser::GetUsername() const {
    return user.username;
}

std::string DiscordUser::GetPingableUsername() const {
    return pingable;
}

std::string DiscordUser::GetIdentifier() const {
    return ident;
}

bool DiscordUser::GetBot() const {
    return user.bot;
}


DiscordMessage::DiscordMessage(SleepyDiscord::Message message, SleepyDiscord::DiscordClient &client)
: client(client)
, message(message)
, userobj(DiscordUser(message.author)) {

}

DiscordMessage::~DiscordMessage() {
    
}

User &DiscordMessage::GetAuthor() {
    return userobj;
}

const User &DiscordMessage::GetConstAuthor() const {
    return userobj;
}

std::string DiscordMessage::GetSourceName() const {
    return "FIXME";
}

std::string DiscordMessage::GetFullSourceName() const {
    return "FIXME";
}

std::string DiscordMessage::GetSourceIdentifier() const  {
    return std::string("Discord_") + message.channelID.string();
}

std::string DiscordMessage::GetContent() const  {
    return message.content;
}

void DiscordMessage::ReturnToSender(std::string content) {
    bool sent = false;
    while (!sent) {
        try {
            client.sendMessage(message.channelID, content);
            sent = true;
        } catch (...) {}
    }
}

}