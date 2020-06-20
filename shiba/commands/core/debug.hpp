#pragma once

#include <sstream>

#include "shiba/types/command.hpp"

namespace Shiba {

    class DebugCommand : public Command {
        public:
            ~DebugCommand() override {}
            
            std::string GetIdentifier() const noexcept override { return "debug"; }
            int GetMinArgs() const noexcept override { return 0; }
            bool StrictMinArgs() const noexcept override { return false; }
            std::string GetShortDescription() const noexcept override { return "Lists stuff"; }
            std::string GetUsage() const noexcept override { return "debug"; }
            int GetPermLevel() const noexcept override { return -1; }
            bool IgnoreBots() const noexcept override { return true; }
            bool IgnoreSelf() const noexcept override { return true; }

            void Trigger(Message &originalMessage, std::vector<std::string> &argv) override {
                std::ostringstream oss;
                oss<<"this->GetIdentifier(): "<<GetIdentifier()<<std::endl;
                oss<<"this->GetMinArgs(): "<<GetMinArgs()<<std::endl;
                oss<<"this->StrictMinArgs(): "<<StrictMinArgs()<<std::endl;
                oss<<"this->GetShortDescription(): "<<GetShortDescription()<<std::endl;
                oss<<"this->GetUsage(): "<<GetUsage()<<std::endl;
                oss<<"this->GetPermLevel(): "<<GetPermLevel()<<std::endl;
                oss<<"this->IgnoreBots(): "<<IgnoreBots()<<std::endl;
                oss<<"this->IgnoreSelf(): "<<IgnoreSelf()<<std::endl;
                oss<<"this->Trigger arguments:"<<std::endl;
                oss<<"originalMessage.GetAuthor().GetIdentifier(): "<<originalMessage.GetAuthor().GetIdentifier()<<std::endl;
                oss<<"originalMessage.GetAuthor().GetBot(): "<<originalMessage.GetAuthor().GetBot()<<std::endl;
                oss<<"originalMessage.GetAuthor().GetUsername(): "<<originalMessage.GetAuthor().GetUsername()<<std::endl;
                oss<<"originalMessage.GetAuthor().GetPingableUsername(): "<<originalMessage.GetAuthor().GetPingableUsername()<<std::endl;
                oss<<"originalMessage.GetContent(): "<<originalMessage.GetContent()<<std::endl;
                oss<<"originalMessage.GetFullSourceName(): "<<originalMessage.GetFullSourceName()<<std::endl;
                oss<<"originalMessage.GetSourceIdentifier(): "<<originalMessage.GetSourceIdentifier()<<std::endl;
                oss<<"originalMessage.GetSourceName(): "<<originalMessage.GetSourceName()<<std::endl;
                oss<<"argv: {";
                bool first = true;
                for (const std::string &str : argv) {
                    if (!first) oss<<", ";
                    oss<<str;
                }
                oss<<'}'<<std::endl;
                originalMessage.ReturnToSender(oss.str());
            }
    };

}