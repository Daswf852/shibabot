#pragma once

#include <string>
#include <vector>

#include "shiba/types/command.hpp"

namespace Shiba {

    class PingCommand : public Command {
        public:
            ~PingCommand() override {}

            std::string GetIdentifier() const override { return "ping"; }
            int GetMinArgs() const override { return 0; }
            bool StrictMinArgs() const override { return true; }
            std::string GetShortDescription() const override { return "pong"; }
            std::string GetUsage() const override { return "ping"; }
            int GetPermLevel() const override { return -1; }
            bool IgnoreBots() const override { return true; }
            bool IgnoreSelf() const override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                originalMessage.ReturnToSender("Pong!");
            }
    };

}