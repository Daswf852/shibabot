#include "spdlog/spdlog.h"

#include "shiba/core/core.hpp"
#include "shiba/core/userManager.hpp"
#include "shiba/frontends/discordFE.hpp"
#include "shiba/commands/core/debug.hpp"
#include "shiba/commands/core/echo.hpp"
#include "shiba/commands/core/ping.hpp"
#include "shiba/modules/genericCommandModule.hpp"

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Beep boop I'm a bot");

    std::unique_ptr<Shiba::CommandModule> coremodule = std::make_unique<Shiba::GenericCommandModule>("core");
    coremodule->RegisterCommand(std::make_unique<Shiba::EchoCommand>());
    coremodule->RegisterCommand(std::make_unique<Shiba::PingCommand>());
    coremodule->RegisterCommand(std::make_unique<Shiba::DebugCommand>());

    Shiba::BotCore core;
    core.AddModule(std::move(coremodule));
    
    core.Load("config.json");

    Shiba::Discord::Frontend discordFE(core, core.GetMiscConfigs().at("discordToken"));
    core.AddFrontend(discordFE);
    
    core.Start();
    core.WaitForStopRequest();

    return 0;
}