#include "bot/discordFE.hpp"

namespace Shiba::Discord {

Frontend::Frontend(BotCore &core, std::string token)
: SleepyDiscord::DiscordClient(token, SleepyDiscord::USER_CONTROLED_THREADS)
, core(core) {

}

Frontend::~Frontend() {
    Stop();
}

void Frontend::Start() {
    spdlog::debug("Discord frontend is starting");
    if (runnerThread.joinable()) return;
    runnerThread = std::thread(&Shiba::Discord::Frontend::RunnerThreadFunction, this);
}

void Frontend::Stop() {
    spdlog::debug("Discord frontend is stopping");
    quit();
    //runnerStopCV.notify_all();
    if (runnerThread.joinable()) runnerThread.join();
}

void Frontend::RunnerThreadFunction() {
    run();
    //std::unique_lock<std::mutex> lock(runnerStopMutex);
    //runnerStopCV.wait(lock);
    //lock.unlock();
    //quit();
}

void Frontend::onMessage(SleepyDiscord::Message message) {
    DiscordMessage dmessage(message, *this);
    std::unique_lock<std::mutex> lock(core.coreMutex);
    core.OnMessage(dmessage);
    lock.unlock();
}


void Frontend::onReady(SleepyDiscord::Ready readyData) {
    core.AddSelfIdentifier(std::string("Discord_") + this->getID().string());
}

}