#include "shiba/modules/genericCommandModule.hpp"

namespace Shiba {

GenericCommandModule::GenericCommandModule(std::string name) : name(name) {
    
}

GenericCommandModule::~GenericCommandModule() {

}

void GenericCommandModule::RegisterCommand(std::unique_ptr<Command> command) {
    identifierList.push_back(command->GetIdentifier());
    commands.insert_or_assign(command->GetIdentifier(), std::move(command));
}

bool GenericCommandModule::ContainsCommand(const std::string &command) const {
    return commands.count(command) > 0;
}

Command &GenericCommandModule::GetCommand(const std::string &command) {
    try {
        return *(commands.at(command));
    } catch (...) { throw CommandNotInModule(command, name); }
}

const Command &GenericCommandModule::GetCommand(const std::string &command) const {
    try {
        return *(commands.at(command));
    } catch (...) { throw CommandNotInModule(command, name); }
}

const std::string &GenericCommandModule::GetName() const {
    return name;
}

const std::vector<std::string> &GenericCommandModule::GetIdentifierList() const {
    return identifierList;
}

}