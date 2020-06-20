#pragma once

#include <string>
#include <vector>

#include "spdlog/spdlog.h"

#include "shiba/core/core.hpp"
#include "shiba/types/command.hpp"
#include "shiba/utils.hpp"

namespace Shiba {

    class GenerateTokenCommand : public Command {
        public:
            GenerateTokenCommand(BotCore &core) : core(core) {};
            ~GenerateTokenCommand() override {}
            
            std::string GetIdentifier() const noexcept override { return "generateToken"; }
            int GetMinArgs() const noexcept override { return 0; }
            bool StrictMinArgs() const noexcept override { return true; }
            std::string GetShortDescription() const noexcept override { return "You know what this does if you want to use it. Otherwise, don't use it"; }
            std::string GetUsage() const noexcept override { return "generateToken"; }
            int GetPermLevel() const noexcept override { return -1; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                std::string token = Utils::GenerateToken();
                core.PushToken(token);
                spdlog::info("NEW TOKEN: {}", token);
                originalMessage.ReturnToSender("Check console");
            }
        
        private:
            BotCore &core;
    };

}