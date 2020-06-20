#include "spdlog/spdlog.h"

#include "shiba/core/core.hpp"
#include "shiba/core/userManager.hpp"
#include "shiba/frontends/discordFE.hpp"
#include "shiba/commands/core/debug.hpp"
#include "shiba/commands/core/echo.hpp"
#include "shiba/commands/core/ping.hpp"
#include "shiba/commands/info/command.hpp"
#include "shiba/commands/info/help.hpp"
#include "shiba/commands/info/module.hpp"
#include "shiba/commands/info/modules.hpp"
#include "shiba/commands/management/claimToken.hpp"
#include "shiba/commands/management/enable.hpp"
#include "shiba/commands/management/generateToken.hpp"
#include "shiba/commands/management/save.hpp"
#include "shiba/commands/management/stop.hpp"
#include "shiba/modules/genericCommandModule.hpp"

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Beep boop I'm a bot");

    Shiba::BotCore core;

    std::unique_ptr<Shiba::CommandModule> coreModule = std::make_unique<Shiba::GenericCommandModule>("core");
    coreModule->RegisterCommand(std::make_unique<Shiba::EchoCommand>());
    coreModule->RegisterCommand(std::make_unique<Shiba::PingCommand>());
    coreModule->RegisterCommand(std::make_unique<Shiba::DebugCommand>());

    std::unique_ptr<Shiba::CommandModule> managementModule = std::make_unique<Shiba::GenericCommandModule>("management");
    managementModule->RegisterCommand(std::make_unique<Shiba::ClaimTokenCommand>(core));
    managementModule->RegisterCommand(std::make_unique<Shiba::EnableCommand>(core));
    managementModule->RegisterCommand(std::make_unique<Shiba::GenerateTokenCommand>(core));
    managementModule->RegisterCommand(std::make_unique<Shiba::SaveCommand>(core));
    managementModule->RegisterCommand(std::make_unique<Shiba::StopCommand>(core));

    std::unique_ptr<Shiba::CommandModule> infoModule = std::make_unique<Shiba::GenericCommandModule>("information");
    infoModule->RegisterCommand(std::make_unique<Shiba::CommandCommand>(core));
    infoModule->RegisterCommand(std::make_unique<Shiba::HelpCommand>());
    infoModule->RegisterCommand(std::make_unique<Shiba::ModuleCommand>(core));
    infoModule->RegisterCommand(std::make_unique<Shiba::ModulesCommand>(core));

    core.AddModule(std::move(coreModule));
    core.AddModule(std::move(managementModule));
    core.AddModule(std::move(infoModule));
    
    core.Load("config.json");

    Shiba::Discord::Frontend discordFE(core, core.GetMiscConfigs().at("discordToken"));
    core.AddFrontend(discordFE);
    
    core.Start();
    core.WaitForStopRequest();

    return 0;
}