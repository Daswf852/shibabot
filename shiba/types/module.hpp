#pragma once

#include <exception>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

#include "shiba/types/command.hpp"

namespace Shiba {

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
            virtual ~CommandModule() {};

            virtual void RegisterCommand(std::unique_ptr<Command> command) = 0;
            virtual bool ContainsCommand(const std::string &command) const = 0;
            virtual Command &GetCommand(const std::string &command) = 0;
            virtual const Command &GetCommand(const std::string &command) const = 0;
            virtual const std::string &GetName() const = 0;
            virtual const std::vector<std::string> &GetIdentifierList() const = 0;
    };

}