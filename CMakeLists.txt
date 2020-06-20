cmake_minimum_required (VERSION 3.6)
set(CMAKE_CXX_STANDARD 17)

project(shibabot)

if(NOT SLEEPY_DISCORD_CMAKE)
	add_subdirectory(submodules/sleepy-discord)
endif()

if(NOT SPDLOG_CMAKE)
	add_subdirectory(submodules/spdlog)
endif()

add_subdirectory(submodules/json)

add_executable(shibabot
    main.cpp
    src/utils.cpp
    src/bot/core.cpp
    src/bot/userManager.cpp
    src/bot/discordBot.cpp
)

target_include_directories(shibabot PRIVATE
    .
    include
    submodules/sleepy-discord/include
    submodules/spdlog/include
    submodules/json/include
)

target_link_libraries(shibabot
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    sleepy-discord
    pthread
)