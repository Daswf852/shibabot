#pragma once

#include <string>
#include <vector>

#include "shiba/core/core.hpp"
#include "shiba/types/command.hpp"

namespace Shiba {

    class StopCommand : public Command {
        public:
            StopCommand(BotCore &core) : core(core) {};
            ~StopCommand() override {}
            
            std::string GetIdentifier() const noexcept override { return "stop"; }
            int GetMinArgs() const noexcept override { return 0; }
            bool StrictMinArgs() const noexcept override { return true; }
            std::string GetShortDescription() const noexcept override { return "Saves the necessary data to files and then stops the bot (on all frontends)"; }
            std::string GetUsage() const noexcept override { return "stop"; }
            int GetPermLevel() const noexcept override { return 4; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                originalMessage.ReturnToSender("Saving and stopping");
                core.Save();
                core.Stop();
            }
        
        private:
            BotCore &core;
    };

}