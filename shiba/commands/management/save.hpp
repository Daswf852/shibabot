#pragma once

#include <string>
#include <vector>

#include "shiba/core/core.hpp"
#include "shiba/types/command.hpp"

namespace Shiba {

    class SaveCommand : public Command {
        public:
            SaveCommand(BotCore &core) : core(core) {};
            ~SaveCommand() override {}
            
            std::string GetIdentifier() const noexcept override { return "save"; }
            int GetMinArgs() const noexcept override { return 0; }
            bool StrictMinArgs() const noexcept override { return true; }
            std::string GetShortDescription() const noexcept override { return "Saves the necessary data to files"; }
            std::string GetUsage() const noexcept override { return "save"; }
            int GetPermLevel() const noexcept override { return 4; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                originalMessage.ReturnToSender("Saving stuffs");
                core.Save();
            }
        
        private:
            BotCore &core;
    };

}