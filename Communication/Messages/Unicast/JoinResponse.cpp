/**
 * @file JoinResponse.cpp
 * @author paul
 * @date 19.03.19
 * @brief JoinResponse @TODO
 */

#include "JoinResponse.hpp"
#include <utility>

namespace communication::messages::unicast {

    auto JoinResponse::getName() -> std::string {
        return "joinResponse";
    }

    JoinResponse::JoinResponse(std::string message) : message{std::move(message)} {}

    auto JoinResponse::getMessage() const -> std::string {
        return this->message;
    }
}
