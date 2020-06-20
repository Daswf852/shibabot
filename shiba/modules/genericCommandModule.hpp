#pragma once

#include "shiba/types/module.hpp"

namespace Shiba {

    class GenericCommandModule : public CommandModule {
        public:
            GenericCommandModule(std::string name);
            ~GenericCommandModule() override;

            void RegisterCommand(std::unique_ptr<Command> command) override;
            bool ContainsCommand(const std::string &command) const override;
            Command &GetCommand(const std::string &command) override;
            const Command &GetCommand(const std::string &command) const override;
            const std::string &GetName() const override;
            const std::vector<std::string> &GetIdentifierList() const override;
        
        private:
            std::string name;
            std::vector<std::string> identifierList;
            std::unordered_map<std::string, std::unique_ptr<Command>> commands;
    };

}