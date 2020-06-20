#pragma once

#include <string>
#include <vector>

#include "shiba/types/message.hpp"

namespace Shiba {

    class Command {
        public:
            virtual ~Command() = default;

            //Returns a command identifier (aka what you write for command)
            //Examples: ping, echo
            virtual std::string GetIdentifier() const = 0;

            //Returns the minimum number of arguments after the command identifier
            //Negative values mean that there will be no argument count checks
            virtual int GetMinArgs() const = 0;

            //Returns wheter the command can use more than GetMinArgs() arguments
            //Ignored if GetMinArgs() < 0
            virtual bool StrictMinArgs() const = 0;

            //Returns a short description of a command
            //Example: (for echo) "echoes back your message"
            virtual std::string GetShortDescription() const = 0;

            //Returns a string that describes how a command is used
            //[] for optional arguments
            //<> for required arguments
            //Example: (for echo) "echo <message>"
            virtual std::string GetUsage() const = 0;

            //Returns the minimum needed permission level
            //anything 0 or below 0 means anyone can execute
            //1: chanop (IRC)/at least moderator (Discord)
            //2: bot director
            //3: bot half operator
            //4: bot operator
            virtual int GetPermLevel() const = 0;

            //Returns wheter or not we ignore ourselves
            //Possible reason as to why we might accept commands from ourselves is to measure ping time
            virtual bool IgnoreSelf() const = 0;

            //Returns wheter or not we ignore other bots
            //Not possible to check on every platform so this might get ignored
            virtual bool IgnoreBots() const = 0;

            //Assumes you did checks beforehand
            virtual void Trigger(Message &originalMessage, std::vector<std::string> &argv) = 0;
    };

}