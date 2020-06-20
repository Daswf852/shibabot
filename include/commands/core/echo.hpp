#pragma once

#include <string>
#include <vector>

#include "types/types.hpp"
#include "utils.hpp"

namespace Shiba {

    class EchoCommand : public Command {
        public:
            std::string GetIdentifier() const noexcept override { return "echo"; }
            int GetMinArgs() const noexcept override { return 1; }
            bool StrictMinArgs() const noexcept override { return false; }
            std::string GetShortDescription() const noexcept override { return "Echoes back what you say"; }
            std::string GetUsage() const noexcept override { return "echo <message>"; }
            int GetPermLevel() const noexcept override { return -1; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                originalMessage.ReturnToSender(Utils::Concatenate(argv, " ", 1));
            }
    };

}