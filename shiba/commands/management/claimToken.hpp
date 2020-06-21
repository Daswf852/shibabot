#pragma once

#include <string>
#include <vector>

#include "shiba/core/core.hpp"
#include "shiba/types/command.hpp"
#include "shiba/utils.hpp"

namespace Shiba {

    class ClaimTokenCommand : public Command {
        public:
            ClaimTokenCommand(BotCore &core) : core(core) {};
            ~ClaimTokenCommand() override {}
            
            std::string GetIdentifier() const noexcept override { return "claimToken"; }
            int GetMinArgs() const noexcept override { return 1; }
            bool StrictMinArgs() const noexcept override { return true; }
            std::string GetShortDescription() const noexcept override { return "You know what this does if you want to use it. Otherwise, don't use it"; }
            std::string GetUsage() const noexcept override { return "claimToken"; }
            int GetPermLevel() const noexcept override { return -1; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                std::string token = argv.at(1);
                if (core.IsTokenValid(token)) {
                    core.ClearTokens();
                    core.SetUserPerm(originalMessage.GetAuthor().GetIdentifier(), 4);
                    originalMessage.ReturnToSender("Claimed");
                    return;
                }
                originalMessage.ReturnToSender("Don't even try >_>");
                ///TODO: add fail2ban on abusers
            }
        
        private:
            BotCore &core;
    };

}