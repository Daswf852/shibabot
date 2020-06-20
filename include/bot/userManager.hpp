#pragma once

#include <string>
#include <unordered_map>

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

#include "types/types.hpp"

namespace Shiba {

    class UserManager {
        public:
            class User {
                public:
                    User(int permLevel = 0)
                    : permLevel(permLevel) {}

                    User(const User &user)
                    : permLevel(user.permLevel) {}

                    ~User() {}
                    
                    int permLevel;
            };

            UserManager();
            ~UserManager();

            nlohmann::json GetJSON() const noexcept;
            void FromJSON(nlohmann::json &j);

            UserManager::User &MakeOrGetUser(std::string identifier) noexcept;
            const UserManager::User &GetUser(std::string identifier) const;
            UserManager::User &GetUser(std::string identifier);
            
        private:
            std::unordered_map<std::string, User> users;
    };

}