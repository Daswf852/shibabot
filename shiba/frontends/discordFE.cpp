#include "shiba/frontends/discordFE.hpp"

namespace Shiba::Discord {

Frontend::Frontend(BotCore &core)
: core(core) {
    std::string token = core.GetConfiguration("discordToken");
    SleepyDiscord::DiscordClient(token, SleepyDiscord::USER_CONTROLED_THREADS);

    if (token == "") {
        spdlog::error("DiscordFE: bad token");
        failed = true;
    }
}

Frontend::~Frontend() {
    Stop();
}

void Frontend::Start() {
    if (failed) return;
    spdlog::debug("DiscordFE: Discord frontend is starting");
    if (runnerThread.joinable()) return;
    runnerThread = std::thread(&Shiba::Discord::Frontend::run, this);
}

void Frontend::Stop() {
    if (failed) return;
    spdlog::debug("DiscordFE: Discord frontend is stopping");
    quit();
    if (runnerThread.joinable()) runnerThread.join();
}

void Frontend::onMessage(SleepyDiscord::Message message) {
    spdlog::info("DiscordFE: New message");
    std::unique_ptr<Message> dmessage = std::make_unique<DiscordMessage>(message, *this);
    core.QueueMessage(std::move(dmessage));
}


void Frontend::onReady(SleepyDiscord::Ready readyData) {
    core.AddSelfIdentifier(std::string("Discord_") + this->getID().string());
}

}