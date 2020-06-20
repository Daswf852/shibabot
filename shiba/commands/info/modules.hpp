#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "shiba/core/core.hpp"
#include "shiba/types/command.hpp"

namespace Shiba {

    class ModulesCommand : public Command {
        public:
            ModulesCommand(BotCore &core) : core(core) {};
            ~ModulesCommand() override {}
            
            std::string GetIdentifier() const noexcept override { return "modules"; }
            int GetMinArgs() const noexcept override { return 0; }
            bool StrictMinArgs() const noexcept override { return true; }
            std::string GetShortDescription() const noexcept override { return "Lists the command modules"; }
            std::string GetUsage() const noexcept override { return "modules"; }
            int GetPermLevel() const noexcept override { return -1; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                std::ostringstream oss;
                oss<<"List of modules: "<<std::endl;

                bool first = true;
                for (const std::unique_ptr<CommandModule> &module : core.GetModules()) {
                    if (!first) oss<<", ";
                    first = false;
                    oss<<module->GetName();
                }

                oss<<std::endl<<"Try doing \"shiba module <module name>\"";

                originalMessage.ReturnToSender(oss.str());
            }
        
        private:
            BotCore &core;
    };

}