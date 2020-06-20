#pragma once

#include <string>
#include <vector>

#include "types/types.hpp"

namespace Shiba {

    class PingCommand : public Command {
        public:
            std::string GetIdentifier() const noexcept override { return "ping"; }
            int GetMinArgs() const noexcept override { return 0; }
            bool StrictMinArgs() const noexcept override { return true; }
            std::string GetShortDescription() const noexcept override { return "pong"; }
            std::string GetUsage() const noexcept override { return "ping"; }
            int GetPermLevel() const noexcept override { return -1; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                originalMessage.ReturnToSender("Pong!");
            }
    };

}