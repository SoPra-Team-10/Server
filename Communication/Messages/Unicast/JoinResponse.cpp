/**
 * @file JoinResponse.cpp
 * @author paul
 * @date 19.03.19
 * @brief Definition of the JoinResponse class
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

    bool JoinResponse::operator==(const JoinResponse &rhs) const {
        return message == rhs.message;
    }

    bool JoinResponse::operator!=(const JoinResponse &rhs) const {
        return !(rhs == *this);
    }

    void to_json(nlohmann::json &j, const JoinResponse &joinResponse) {
        j["message"] = joinResponse.getMessage();
    }

    void from_json(const nlohmann::json &j, JoinResponse &joinResponse) {
        joinResponse = JoinResponse{
            j.at("message").get<std::string>()
        };
    }
}
