#pragma once

#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

#include "bot/userManager.hpp"
#include "types/types.hpp"
#include "utils.hpp"

namespace Shiba {
    class BotCore {
        public:
            BotCore();
            ~BotCore();

            void Save();
            void Load();

            void AddSelfIdentifier(std::string identifier);
            void AddModule(CommandModule &module);

            void AddFrontend(Frontend &fe);
            void Start();
            void WaitForStopRequest();
            void Stop();

            void OnMessage(Message &message);
            Command &GetCommand(std::string identifier);
            bool ProcessCommand(bool self, bool bot, std::vector<std::string> &argv, Message &message);
            bool ProcessSemiCommand(bool self, bool bot, std::vector<std::string> &argv, Message &message);
            bool ProcessPlainMessage(bool self, bool bot, Message &message);

            std::mutex coreMutex;

        private:
            const std::string prefix = "shiba";

            const std::string enabledChannelsSaveFile = "enabledchans.json";
            const std::string userManagerSaveFile = "usermgr.json";

            std::vector<std::string> selfIdentifiers;
            std::vector<std::string> enabledChannels;

            std::vector<std::reference_wrapper<CommandModule>> modules;

            std::list<std::string> tokenQueue;

            std::vector<std::reference_wrapper<Frontend>> frontends;

            std::condition_variable stopRequestCV;
            std::mutex stopRequestMutex;
            bool stopRequest = false;

            UserManager userManager;
    };
}