#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "shiba/core/core.hpp"
#include "shiba/types/command.hpp"

namespace Shiba {

    class ModuleCommand : public Command {
        public:
            ModuleCommand(BotCore &core) : core(core) {};
            ~ModuleCommand() override {}
            
            std::string GetIdentifier() const noexcept override { return "module"; }
            int GetMinArgs() const noexcept override { return 1; }
            bool StrictMinArgs() const noexcept override { return true; }
            std::string GetShortDescription() const noexcept override { return "Lists the contents of a command module"; }
            std::string GetUsage() const noexcept override { return "module [module]"; }
            int GetPermLevel() const noexcept override { return -1; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                std::string moduleName = argv.at(1);
                std::optional<std::reference_wrapper<const std::unique_ptr<CommandModule>>> optCommandModule = std::nullopt;
                for (const std::unique_ptr<CommandModule> &module : core.GetModules()) {
                    if (module->GetName() == moduleName) optCommandModule = module;
                }

                if (!optCommandModule.has_value()) {
                    originalMessage.ReturnToSender("No such module could be found");
                } else {
                    const std::unique_ptr<CommandModule> &module = optCommandModule.value();

                    std::ostringstream oss;

                    oss<<"Module \""<<module->GetName()<<"\""<<std::endl
                    <<"Commands (unordered): ";

                    bool first = true;
                    for (const std::string &str : module->GetIdentifierList()) {
                        if (!first) oss<<", ";
                        first = false;
                        oss<<str;
                    }

                    oss<<std::endl<<"Try \"shiba command <command>\"";

                    originalMessage.ReturnToSender(oss.str());
                }
            }
        
        private:
            BotCore &core;
    };

}