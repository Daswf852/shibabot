#pragma once

#include <mutex>
#include <string>
#include <thread>

#include "sleepy_discord/sleepy_discord.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "shiba/core/core.hpp"
#include "shiba/frontends/discordtypes.hpp"

namespace Shiba::Discord {

    class Frontend : public SleepyDiscord::DiscordClient, public Shiba::Frontend {
        public:
            Frontend(BotCore &core);
            ~Frontend();

            void Start() override;
            void Stop() override;

        private:
            BotCore &core;

            bool failed = false;

            std::thread runnerThread;
            std::condition_variable runnerStopCV;
            std::mutex runnerStopMutex;

            void onMessage(SleepyDiscord::Message message) override;
            void onReady(SleepyDiscord::Ready readyData) override;
    };

}