#include "shiba/core/userManager.hpp"

namespace Shiba {

UserManager::UserManager() {
    spdlog::info("Making a new user manager");
}

UserManager::~UserManager() {

}

nlohmann::json UserManager::GetJSON() const {
    spdlog::info("Getting json of the user manager");
    nlohmann::json j;
    for (auto &user : users) {
        j[user.first] = {user.second.permLevel};
    }
    return j;
}

void UserManager::FromJSON(nlohmann::json &j) {
    spdlog::info("Trying to load json for the user manager");

    if (!j.is_object()) {
        spdlog::error("Bad json");
        return;
    }

    for (auto &[k, v] : j.items()) {
        bool badEntry = false;
        
        ///TODO: deduce element size from UserManager::User members later
        badEntry |= ((!v.is_array()) || (v.size() != 1));
        if (!badEntry) { //if the array is invalid we cant process other elements
            badEntry |= (!v[0].is_number_integer());
        }

        if (badEntry) {
            spdlog::error("Skipping bad entry in {}", k);
            continue;
        }

        UserManager::User user(v[0]);
        users.insert({k, std::move(user)});
    }
}

UserManager::User &UserManager::MakeOrGetUser(std::string identifier) {
    if (users.find(identifier) == users.end()) {
        User newUser(0);
        users.insert({identifier, std::move(newUser)});
    }
    return GetUser(identifier);
}

const UserManager::User &UserManager::GetUser(std::string identifier) const {
    try {
        return users.at(identifier);
    } catch (std::out_of_range &e) {
        throw e;
    }
}

UserManager::User &UserManager::GetUser(std::string identifier) {
    try {
        return users.at(identifier);
    } catch (std::out_of_range &e) {
        throw e;
    }
}

}