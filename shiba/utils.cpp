#include "shiba/utils.hpp"

namespace Shiba::Utils {
    std::vector<std::string> Tokenise(const std::string &string) {
        std::string current;
        std::istringstream iss(string);
        std::vector<std::string> tokens;
        while (iss >> current) {
            tokens.push_back(current);
        }
        return tokens;
    }

    std::vector<std::string> Tokenise(const std::string &string, std::string regex) {
        std::regex re(regex);

        return std::vector<std::string> (
            std::sregex_token_iterator{string.cbegin(), string.cend(), re, -1},
            std::sregex_token_iterator{}
        );
    }

    std::string Concatenate(const std::vector<std::string> &tokens, char fill, size_t offset) {
        std::string fillString;
        fillString.push_back(fill);
        return Concatenate(tokens, fillString);
    }

    std::string Concatenate(const std::vector<std::string> &tokens, const std::string &fill, size_t offset) {
        std::ostringstream oss;
        
        bool first = true;

        for (std::vector<std::string>::const_iterator cit = tokens.begin() + offset; cit < tokens.end(); cit++) {
            if (!first) oss<<fill;
            first = false;
            oss<<*cit;
        }

        return oss.str();
    }

    bool StartsWith(const std::string &strToCheck, const std::string &prefix) {
        return strToCheck.rfind(prefix, 0) == 0;
    }

    void ErasePrefix(std::string &source, const std::string &prefix) {
        source.erase(source.begin(), source.begin() + prefix.size());
    }

    std::string GenerateToken() {
        const std::string_view charset("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, charset.size() - 1);
    
        std::string token("          ");

        for (std::string::iterator it = token.begin(); it != token.end(); it++) {
            *it = charset.at(dist(gen));
        }

        return token;
    }

    bool LoadJSON(nlohmann::json &j, const std::string &path) {
        std::ifstream ifs(path);
        if (!ifs) {
            spdlog::error("Could not open path \"{}\" to read JSON from");
            return false;
        }

        std::string tmpline;
        std::string jsonstr;
        while (std::getline(ifs, tmpline)) jsonstr += tmpline;

        try {
            j = nlohmann::json::parse(jsonstr);
        } catch (...) {
            spdlog::error("Failed to parse JSON string:\n{}", jsonstr);
            return false;
        }

        ifs.close();
        return true;
    }

    bool SaveJSON(const nlohmann::json &j, const std::string &path) {
        std::ofstream ofs(path);
        if (!ofs) {
            spdlog::error("Could not open path \"{}\" to write JSON to", path);
            return false;
        }

        ofs<<j.dump();

        ofs.close();
        return true;
    }

    bool TryCreateDirectory(const std::string &path) {
        if (!std::filesystem::exists(path)) {
            try {
                std::filesystem::create_directory(path);
                return true;
            } catch (std::filesystem::filesystem_error &e) {
                spdlog::error("Could not make the directory \"{}\", e.what():\n{}", path, e.what());
                return false;
            }
        } else { //It exists but might not be a directory
            try {
                return std::filesystem::is_directory(path);
            } catch (std::filesystem::filesystem_error &e) {
                spdlog::error("Could not verify if the file \"{}\" is actually a directory, e.what():\n{}", path, e.what());
                return false;
            }
        }
        return true; //shouldn't be possible to reach
    }

};