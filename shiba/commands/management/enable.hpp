#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "spdlog/spdlog.h"

#include "shiba/core/core.hpp"
#include "shiba/types/command.hpp"

namespace Shiba {

    class EnableCommand : public Command {
        public:
            EnableCommand(BotCore &core) : core(core) {};
            ~EnableCommand() override {}
            
            std::string GetIdentifier() const noexcept override { return "enable"; }
            int GetMinArgs() const noexcept override { return 0; }
            bool StrictMinArgs() const noexcept override { return true; }
            std::string GetShortDescription() const noexcept override { return "Enables the bot on a channel"; }
            std::string GetUsage() const noexcept override { return "enable"; }
            int GetPermLevel() const noexcept override { return -1; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                spdlog::info("Adding enabled channel {}", originalMessage.GetSourceIdentifier());
                core.AddEnabledChannel(originalMessage.GetSourceIdentifier());
                std::ostringstream oss;
                oss<<"Enabled in this channel ("<<originalMessage.GetSourceIdentifier()<<')';
                originalMessage.ReturnToSender(oss.str());
            }
        
        private:
            BotCore &core;
    };

}