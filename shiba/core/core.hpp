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

            //To be called single-threadedly from main

            //Saves bot data files
            void Save();
            //Loads bot configuration and data
            void Load(std::string configPath);

            //Adds a command module to the core
            void AddModule(std::unique_ptr<CommandModule> module);
            //Adds a platform frontend to the core
            void AddFrontend(Frontend &fe);

            //Starts the bot on all frontends
            void Start();
            //Waits for Stop() to be called
            void WaitForStopRequest();
            //Stops all frontends and the core
            void Stop();

            //To be called from commands and frontends

            //Adds a bot's self identifier to later check if a message's author is us
            void AddSelfIdentifier(std::string identifier);
            //Adds a channel (on any platform) on which the bot works on
            void AddEnabledChannel(std::string channel);
            //Queues a message to be processed
            void QueueMessage(std::unique_ptr<Message> message);
            //Pushes an operator token
            //If the amount of tokens is more than 20, removes tokens until it is 20
            void PushToken(std::string token);
            //Checks if an operator token is valid
            bool IsTokenValid(const std::string &token);
            //Clears all operator tokens
            void ClearTokens();

            //Tidy up below
            const std::unordered_map<std::string, std::string> &GetMiscConfigs() const;
            UserManager &GetUserManager();
            const std::vector<std::unique_ptr<CommandModule>> &GetModules();
            //ugly ugly and ugly
            const Command &GetCommand(std::string identifier) const;

        private:
            // Private member functions

            void OnMessage(Message &message);
            Command &GetCommand(std::string identifier);
            bool ProcessCommand(bool self, bool bot, std::vector<std::string> &argv, Message &message);
            bool ProcessPlainMessage(bool self, bool bot, Message &message);

            void MessageWorkerThreadFunction();
            void NotifyMessageWorker();

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
            
            std::mutex selfIdentifiersMutex;
            std::vector<std::string> selfIdentifiers;

            std::mutex enabledChannelsMutex;
            std::vector<std::string> enabledChannels;

            // Modules, commands, users etc.

            std::vector<std::unique_ptr<CommandModule>> modules;

            std::mutex tokenQueueMutex;
            std::list<std::string> tokenQueue;

            UserManager userManager;

            // Worker related stuff

            bool stopMessageWorker = false;
            std::thread messageWorkerThread;

            std::mutex messageQueueMutex;
            std::queue<std::unique_ptr<Message>> messageQueue;

            std::mutex messageNotifyMutex;
            std::condition_variable messageNotifyCV;

    };
}