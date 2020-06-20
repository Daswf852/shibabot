#include "shiba/core/core.hpp"

namespace Shiba {

BotCore::BotCore() : userManager(UserManager()) {
    spdlog::info("New bot core is getting initialised");
}

BotCore::~BotCore() {

}

void BotCore::Save() {
    spdlog::info("Save is called");
    
    /* directory structure:
     * token
     * botconfig.json
     * enabledchans.json
     * usermgr.json
     * Backups/
     *  0/
     *   enabledchans.json
     *   usermgr.json
     *  1/
     *   enabledchans.json
     *   usermgr.json
     */

    //Check if Backups/ exist, if not try to create it
    if (Utils::TryCreateDirectory("Backups")) {
        int biggestID = 0;
        std::filesystem::directory_iterator it("Backups");
        for (const std::filesystem::directory_entry &entry : it) {
            int tempID = -1;
            try {
                tempID = std::stoi(entry.path().filename());
            } catch (...) {
                spdlog::warn("Bad file inside Bakcups with the file name \"{}\"", entry.path().filename().string());
                continue;
            }
            biggestID = (tempID > biggestID) ? tempID : biggestID;
        }
        ++biggestID;

        std::string newBackupPathStr = std::string("Backups/") + std::to_string(biggestID) + std::string("/");
        if (Utils::TryCreateDirectory(newBackupPathStr)) {
            std::error_code ec;
            std::filesystem::rename(enabledChannelsSaveFile, newBackupPathStr+enabledChannelsSaveFile, ec);
            std::filesystem::rename(userManagerSaveFile, newBackupPathStr+userManagerSaveFile, ec);
        } else {
            spdlog::error("Could not make the new backup directory with path \"{}\"", newBackupPathStr);
        }
    } else {
        spdlog::error("Could not access the Backups directory");
    }

    spdlog::info("Saving channels list");
    if (!Utils::SaveJSON(nlohmann::json(enabledChannels), enabledChannelsSaveFile)) {
        spdlog::error("Could not save enabled channels list");
    }

    spdlog::info("Saving user manager");
    if (!Utils::SaveJSON(userManager.GetJSON(), userManagerSaveFile)) {
        spdlog::error("Could not save user manager data");
    }
}

void BotCore::Load(std::string configPath) {
    spdlog::info("Loading from json files");

    spdlog::info("Loading bot config");
    nlohmann::json botConfigJSON;
    if (Utils::LoadJSON(botConfigJSON, configPath)) {
        try {
            prefix = botConfigJSON["prefix"];
            enabledChannelsSaveFile = botConfigJSON["chanlist"];
            userManagerSaveFile = botConfigJSON["userlist"];
            
            nlohmann::json miscConfigsObject = botConfigJSON["miscConfigs"];
            for (auto &[k, v] : miscConfigsObject.items()) {
                spdlog::debug("New misc config: \"{}\": \"{}\"", k, v);
                miscConfigs.insert_or_assign(k, v);
            }

        } catch (...) {
            spdlog::error("Bad bot config! using defaults");
            prefix = "shiba";
            enabledChannelsSaveFile = "enabledchans.json";
            userManagerSaveFile = "usermgr.json";
            miscConfigs = {};
        }
    } else {
        spdlog::error("Could not load bot configuration, using defaults");
    }

    spdlog::info("Loading enabled channels");
    nlohmann::json chanListJSON;
    if (Utils::LoadJSON(chanListJSON, enabledChannelsSaveFile)) {
        if (!chanListJSON.is_array()) {
            spdlog::error("Bad JSON file");
            return;
        }

        for (auto &elem : chanListJSON) {
            if (!elem.is_string()) {
                spdlog::warn("Bad element in json, skipping:\n{}", elem);
                continue;
            }
            enabledChannels.push_back(elem);
        }
    }

    nlohmann::json userManagerJSON;
    if (Utils::LoadJSON(userManagerJSON, userManagerSaveFile)) {
        userManager.FromJSON(userManagerJSON);
    }
}

void BotCore::AddSelfIdentifier(std::string identifier) {
    spdlog::info("Appending new identifier: {}", identifier);
    selfIdentifiers.push_back(identifier);
}

void BotCore::AddModule(std::unique_ptr<CommandModule> module) {
    modules.push_back(std::move(module));
}

void BotCore::AddFrontend(Frontend &fe) {
    frontends.push_back(fe);
}

void BotCore::Start() {
    spdlog::info("Starting frontends");
    for (Frontend &fe : frontends) {
        fe.Start();
    }
}

void BotCore::WaitForStopRequest() {
    spdlog::info("Waiting for a stop request");
    std::unique_lock<std::mutex> lock(stopRequestMutex);
    stopRequestCV.wait(lock);
    lock.unlock();
    spdlog::info("Got stop request, calling Stop()");
    Stop();
}

void BotCore::Stop() {
    spdlog::info("Notifying all listeners on stopRequestCV");
    stopRequestCV.notify_all();
    spdlog::info("Calling Stop() on all frontends");
    for (Frontend &fe : frontends) {
        fe.Stop();
    }
}

const std::unordered_map<std::string, std::string> &BotCore::GetMiscConfigs() const {
    return miscConfigs;
}

void BotCore::OnMessage(Message &message) {
    std::string authorident = message.GetConstAuthor().GetIdentifier();
    spdlog::debug("New message from {}", authorident);

    //We could've either one of these:
    //command message
    //semi-command message 
    //regular message

    std::vector<std::string> argv = Utils::Tokenise(message.GetContent());

    bool self = false;
    for (const std::string &str : selfIdentifiers) {
        if (str == authorident) self = true;
    }
    bool bot = (self ? true : message.GetConstAuthor().GetBot());
    bool processed = false;
    bool enabled = false;
    for (const std::string &str : enabledChannels) {
        if (str == message.GetSourceIdentifier()) enabled = true;
    }
    
    spdlog::debug("Processing message with the current variables:");
    spdlog::debug("self: {}", self);
    spdlog::debug("bot: {}", bot);
    spdlog::debug("enabled: {}", enabled);

    if (enabled && argv.at(0) == prefix) {
        std::vector<std::string> argvAfterStart(argv.begin()+1, argv.end());
        if (!processed) processed = ProcessCommand(self, bot, argvAfterStart, message);
        if (processed) spdlog::debug("Processed the message in ProcessCommand()");
    }

    //if (!processed) processed = ProcessSemiCommand(self, bot, argv, message);
    //if (processed) spdlog::debug("Processed the message in ProcessSemiCommand()");
    if (!processed) processed = ProcessPlainMessage(self, bot, message);
    if (processed) spdlog::debug("Processed the message in ProcessPlainMessage()");

    if (!processed) {
        spdlog::debug("Didn't process the message");
    }
}

Command &BotCore::GetCommand(std::string identifier) {
    std::optional<std::reference_wrapper<Command>> foundCommand;
    
    for (std::unique_ptr<CommandModule> &module : modules) {
        try {
            foundCommand = module->GetCommand(identifier);
        } catch (...) { }
    }

    if (!foundCommand.has_value()) throw CommandNotFound(identifier);
    else return foundCommand.value();
}

bool BotCore::ProcessCommand(bool self, bool bot, std::vector<std::string> &argv, Message &message) {
    spdlog::debug("Called processCommand({}, {}, argv.size() = {}, message)", self, bot, argv.size());
    if (argv.size() == 0) return false;
    //if (argv.at(0) != prefix) return false;

    std::string identifier = argv.at(0);

    spdlog::debug("Command identifier appears to be {}", identifier);

    std::optional<std::reference_wrapper<Command>> optCommand;
    try {
        optCommand = GetCommand(identifier);
    } catch (...) {
        spdlog::debug("Command with identifier \"{}\" could not be found", identifier);
        return false;
    }
    Command &command = optCommand.value();

    //Check requirements
    if (bot && command.IgnoreBots()) return false;
    if (self && command.IgnoreSelf()) return false;
    if ((argv.size() - 1) < command.GetMinArgs()) return false;
    if (command.StrictMinArgs() && ((argv.size() - 1) != command.GetMinArgs())) return false;
    if (command.GetPermLevel() > userManager.MakeOrGetUser(message.GetAuthor().GetIdentifier()).permLevel) {
        message.ReturnToSender("Your permission level is insufficent");
        return false;
    }

    //Execute command
    command.Trigger(message, argv);

    return false;
}

bool BotCore::ProcessPlainMessage(bool self, bool bot, Message &message) {
    return false;
}

void BotCore::PushToken(std::string token) {
    while (tokenQueue.size() >= 20) tokenQueue.pop_back();
    tokenQueue.push_front(token);
}

bool BotCore::IsTokenValid(const std::string &token) {
    for (const std::string &str : tokenQueue) {
        if (str == token) {
            return true;
        }
    }
    return false;
}

void BotCore::ClearTokens() {
    tokenQueue = std::list<std::string>();
}

UserManager &BotCore::GetUserManager() {
    return userManager;
}

const std::vector<std::unique_ptr<CommandModule>> &BotCore::GetModules() {
    return modules;
}

void BotCore::AddEnabledChannel(std::string channel) {
    enabledChannels.push_back(channel);
}


}