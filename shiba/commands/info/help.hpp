#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "shiba/core/core.hpp"
#include "shiba/types/command.hpp"

namespace Shiba {

    class HelpCommand : public Command {
        public:
            HelpCommand() {};
            ~HelpCommand() override {}
            
            std::string GetIdentifier() const noexcept override { return "help"; }
            int GetMinArgs() const noexcept override { return 0; }
            bool StrictMinArgs() const noexcept override { return false; }
            std::string GetShortDescription() const noexcept override { return "Helps you out with the bot"; }
            std::string GetUsage() const noexcept override { return "help"; }
            int GetPermLevel() const noexcept override { return -1; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                std::ostringstream oss;
                oss<<"Usage:"<<std::endl
                   <<"shiba [command] [arguments]"<<std::endl
                   <<"try shiba modules for command modules"<<std::endl
                   <<"No output will be given for commands other than this if you enter a bad number of arguments";
                originalMessage.ReturnToSender(oss.str());
            }
        
        private:
    };

}