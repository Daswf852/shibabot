#pragma once

#include <filesystem>
#include <fstream>
#include <random>
#include <regex>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

namespace Shiba::Utils {
    std::vector<std::string> Tokenise(const std::string &string);
    std::vector<std::string> Tokenise(const std::string &string, std::string regex);
    std::string Concatenate(const std::vector<std::string> &tokens, char fill = ' ', size_t offset = 0);
    std::string Concatenate(const std::vector<std::string> &tokens, const std::string &fill, size_t offset = 0);
    bool StartsWith(const std::string &strToCheck, const std::string &prefix);
    void ErasePrefix(std::string &source, const std::string &prefix);
    std::string GenerateToken();
    bool LoadJSON(nlohmann::json &j, const std::string &path);
    bool SaveJSON(const nlohmann::json &j, const std::string &path);
    //Returns true if directory exists or could be created
    bool TryCreateDirectory(const std::string &path);
};