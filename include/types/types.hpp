#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>

/* The purpose of these abstract classes is to have a consistent way of
 * accessing message and user information from the bot core. Bot core itself
 * won't send messages.
 */

namespace Shiba {

    //An abstract class that contains information about a user
    class User {
        public:
            virtual ~User() = default;

            //Returns a human readable username which tries not to ping the respective user.
            //IRC: daswf852 (pings)
            //Discord: daswf852 (doesn't ping)
            virtual std::string GetUsername() const = 0;

            //Returns a string which tries to ping a user. Might or might not be readable.
            //IRC: daswf852 (pings, readable)
            //Discord: <@254628026380320768> (pings, not readable by itself)
            virtual std::string GetPingableUsername() const = 0;

            //Returns a string which uniquely identifies a user
            //IRC: IRC_freenode_daswf852
            //Discord: Discord_254628026380320768
            virtual std::string GetIdentifier() const = 0;

            //Returns wheter or not a user is a bot
            //Might not always be accurate or possible (to get)
            virtual bool GetBot() const = 0;
    };

    //An abstract class that contains information about a received message
    //All messages are supposed to be passed off as a Message * to the bot core.
    class Message {
        public:
            virtual ~Message() = default;

            //Returns a User object for whoever sent the message
            virtual User &GetAuthor() = 0;
            virtual const User &GetConstAuthor() const = 0;

            //Returns a readable string of where the message was sent from
            //IRC: ##hellomouse
            //Discord: #general
            virtual std::string GetSourceName() const = 0;

            //Returns a readable string of where the message was sent from
            //More descriptive than GetSourceName()
            //IRC: freenode ##hellomouse
            //Discord: Hellomouse #general
            virtual std::string GetFullSourceName() const = 0;

            //Returns an identifier of where the message was sent from
            //IRC: IRC_freenode_##hellomouse
            //Discord: Discord_271781178296500235
            virtual std::string GetSourceIdentifier() const = 0;

            //Returns an identifier of where the message generally was from
            //IRC: IRC_freenode
            //Discord: Discord_271781178296500235
            //virtual std::string GetGeneralSourceIdentifier() const = 0;

            //Returns the text contents of the message
            //For advanced text features for a platform, cast this Message object to a more specific one such as DiscordMessage
            virtual std::string GetContent() const = 0;

            //Returns a reply to where the message was received from
            virtual void ReturnToSender(std::string content) = 0;
    };

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

    struct CommandNotInModule : public std::exception {
        std::string identifier = "";
        std::string moduleName = "";

        std::string whatString;

        CommandNotInModule(std::string identifier, std::string moduleName)
        : identifier(identifier)
        , moduleName(moduleName) {
            std::ostringstream oss;
            oss<<"Identifier "<<identifier<<" not found in module "<<moduleName<<'.';
            whatString = std::string(oss.str());
        }

        const char *what () const throw() override {
            return whatString.c_str();
        }
    };

    struct CommandNotFound : public std::exception {
        std::string identifier = "";
        std::string whatString = "";

        CommandNotFound(std::string identifier) : identifier(identifier) {
            std::ostringstream oss;
            oss<<"Command not found: "<<identifier;
            whatString = oss.str();
        }

        const char *what() const throw() override {
            return whatString.c_str();
        }
    };

    class CommandModule {
        public:
            CommandModule(std::string name) : name(name) {

            }

            ~CommandModule() {

            }

            void RegisterCommand(Command &command) {
                //if (commands.count(command.GetIdentifier())) commands.erase(command.GetIdentifier());
                //commands.insert({command.GetIdentifier(), std::reference_wrapper<Command>(command)});
                commands.insert_or_assign(command.GetIdentifier(), std::reference_wrapper<Command>(command));
            }

            bool ContainsCommand(const std::string &command) const {
                return commands.count(command);
            }

            Command &GetCommand(const std::string &command) {
                try {
                    return commands.at(command);
                } catch (...) { throw CommandNotInModule(command, name); }
            }

            const Command &GetConstCommand(const std::string &command) const {
                try {
                    return commands.at(command);
                } catch (...) { throw CommandNotInModule(command, name); }
            }

            std::string GetName() const {
                return name;
            }

            const std::unordered_map<std::string, std::reference_wrapper<Command>> &GetConstCommands() const {
                return commands;
            }

        private:
            std::string name;
            std::unordered_map<std::string, std::reference_wrapper<Command>> commands;
    };

    class Frontend {
        public:
            virtual ~Frontend() = default;
            virtual void Start() = 0;
            virtual void Stop() = 0;
    };

}