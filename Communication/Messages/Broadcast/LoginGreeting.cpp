/**
 * @file LoginGreeting.cpp
 * @author paul
 * @date 19.03.19
 * @brief LoginGreeting @TODO
 */

#include "LoginGreeting.hpp"

#include <utility>

namespace communication::messages::broadcast {
    LoginGreeting::LoginGreeting(std::string userName) : userName{std::move(userName)} {}

    auto LoginGreeting::getUserName() const -> std::string {
        return this->userName;
    }

    auto LoginGreeting::getName() -> std::string {
        return "loginGreeting";
    }
}
