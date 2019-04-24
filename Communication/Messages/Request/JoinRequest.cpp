#include <utility>

#include <utility>

/**
 * @file JoinRequest.cpp
 * @author paul
 * @date 19.03.19
 * @brief JoinRequest @TODO
 */

#include "JoinRequest.hpp"

namespace communication::messages::request {
    JoinRequest::JoinRequest(std::string lobby, std::string userName, std::string password,
                             bool isArtificialIntelligence, std::optional<std::string> mods) :
                             lobby{std::move(lobby)}, userName{std::move(userName)}, password{std::move(password)},
                             isArtificialIntelligence{isArtificialIntelligence}, mods{std::move(mods)} {}

    auto JoinRequest::getLobby() const -> std::string {
        return this->lobby;
    }

    auto JoinRequest::getUserName() const -> std::string {
        return this->userName;
    }

    auto JoinRequest::getPassword() const -> std::string {
        return this->password;
    }

    auto JoinRequest::getIsAi() const -> bool {
        return this->isArtificialIntelligence;
    }

    auto JoinRequest::getMods() const -> std::optional<std::string> {
        return this->mods;
    }

    auto JoinRequest::getName() -> std::string {
        return "joinRequest";
    }
}
