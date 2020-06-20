#pragma once

#include <mutex>
#include <string>
#include <thread>

#include "sleepy_discord/sleepy_discord.h"

#include "bot/core.hpp"
#include "types/discordtypes.hpp"

namespace Shiba::Discord {

    class Frontend : public SleepyDiscord::DiscordClient, public Shiba::Frontend {
        public:
            Frontend(BotCore &core, std::string token);
            ~Frontend();

            void Start() override;
            void Stop() override;

        private:
            BotCore &core;

            std::thread runnerThread;
            std::condition_variable runnerStopCV;
            std::mutex runnerStopMutex;
            void RunnerThreadFunction();

            void onMessage(SleepyDiscord::Message message) override;
            void onReady(SleepyDiscord::Ready readyData) override;
    };

}