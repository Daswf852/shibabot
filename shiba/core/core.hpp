#pragma once

#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

#include "shiba/types/command.hpp"
#include "shiba/types/frontend.hpp"
#include "shiba/types/message.hpp"
#include "shiba/types/module.hpp"
#include "shiba/core/userManager.hpp"
#include "shiba/utils.hpp"

namespace Shiba {
    class BotCore {
        public:
            BotCore();
            ~BotCore();

            void Save();
            void Load(std::string configPath);

            void AddSelfIdentifier(std::string identifier);
            void AddModule(std::unique_ptr<CommandModule> module);

            void AddFrontend(Frontend &fe);
            void Start();
            void WaitForStopRequest();
            void Stop();

            const std::unordered_map<std::string, std::string> &GetMiscConfigs() const;

            void OnMessage(Message &message);
            Command &GetCommand(std::string identifier);
            bool ProcessCommand(bool self, bool bot, std::vector<std::string> &argv, Message &message);
            bool ProcessSemiCommand(bool self, bool bot, std::vector<std::string> &argv, Message &message);
            bool ProcessPlainMessage(bool self, bool bot, Message &message);

            void PushToken(std::string token);
            bool IsTokenValid(const std::string &token);
            void ClearTokens();
            
            UserManager &GetUserManager();
            const std::vector<std::unique_ptr<CommandModule>> &GetModules();

            void AddEnabledChannel(std::string channel);

            std::mutex coreMutex; //TO BE REMOVED

        private:
            // Bot configuration

            //Key-value pair of misc. configurations
            //Example: {"discordToken", "0123ABCD..."}
            std::unordered_map<std::string, std::string> miscConfigs;

            std::string prefix = "shiba";

            std::string enabledChannelsSaveFile = "enabledchans.json";
            std::string userManagerSaveFile = "usermgr.json";

            // Frontend related members

            std::vector<std::reference_wrapper<Frontend>> frontends;

            std::condition_variable stopRequestCV;
            std::mutex stopRequestMutex;
            
            std::vector<std::string> selfIdentifiers;
            std::vector<std::string> enabledChannels;

            // Modules, commands, users etc.

            std::vector<std::unique_ptr<CommandModule>> modules;

            std::list<std::string> tokenQueue;

            UserManager userManager;

            // Worker related stuff

            void MessageWorkerThreadFunction();
            void QueueMessage(std::unique_ptr<Message> message);
            void NotifyMessageWorker();

            std::queue<std::unique_ptr<Message>> messageQueue;

            std::thread messageWorkerThread;

    };
}