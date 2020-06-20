#include "spdlog/spdlog.h"

#include "bot/core.hpp"
#include "bot/userManager.hpp"
#include "bot/discordFE.hpp"
#include "commands/core/echo.hpp"
#include "commands/core/ping.hpp"
#include "types/types.hpp"

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Beep boop I'm a bot");

    Shiba::CommandModule coremodule("core");
    Shiba::EchoCommand command_echo;
    coremodule.RegisterCommand(command_echo);
    Shiba::PingCommand command_ping;
    coremodule.RegisterCommand(command_ping);

    Shiba::BotCore core;
    core.AddModule(coremodule);
    
    core.Load();

    std::ifstream ifs("token");
    assert(ifs);
    std::string token;
    std::getline(ifs, token);
    ifs.close();

    Shiba::Discord::Frontend discordFE(core, token);
    core.AddFrontend(discordFE);
    
    core.Start();
    core.WaitForStopRequest();

    return 0;
}