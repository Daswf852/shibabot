#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "shiba/core/core.hpp"
#include "shiba/types/command.hpp"

namespace Shiba {

    class CommandCommand : public Command {
        public:
            CommandCommand(BotCore &core) : core(core) {};
            ~CommandCommand() override {}
            
            std::string GetIdentifier() const noexcept override { return "command"; }
            int GetMinArgs() const noexcept override { return 1; }
            bool StrictMinArgs() const noexcept override { return true; }
            std::string GetShortDescription() const noexcept override { return "Shows information about a command"; }
            std::string GetUsage() const noexcept override { return "command [command]"; }
            int GetPermLevel() const noexcept override { return -1; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                std::string identifier = argv.at(1);

                std::optional<std::reference_wrapper<Command>> optCommand;
                try {
                    optCommand = core.GetCommand(identifier);
                } catch (...) {
                    spdlog::debug("Command with identifier \"{}\" could not be found", identifier);
                    originalMessage.ReturnToSender("No such command could be found");
                    return;
                }
                Command &command = optCommand.value();

                std::ostringstream oss;
                oss<<"Command: "<<command.GetIdentifier()<<std::endl
                   <<"Usage: "<<command.GetUsage()<<std::endl
                   <<"Description: "<<command.GetShortDescription()<<std::endl
                   <<(command.StrictMinArgs() ? "Strict" : "Minimum")<<" argument count: "<<command.GetMinArgs()<<std::endl
                   <<"Permission level: "<<command.GetPermLevel();
                originalMessage.ReturnToSender(oss.str());
            }
        
        private:
            BotCore &core;
    };

}