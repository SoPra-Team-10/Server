#include <utility>

/**
 * @file PauseRequest.cpp
 * @author paul
 * @date 19.03.19
 * @brief PauseRequest @TODO
 */

#include "PauseRequest.hpp"

namespace communication::messages::request {

    auto PauseRequest::getName() -> std::string {
        return "pauseRequest";
    }

    PauseRequest::PauseRequest(std::string message) : message(std::move(message)) {}

    std::string PauseRequest::getMessage() const {
        return message;
    }

    void to_json(nlohmann::json &j, const PauseRequest &pauseRequest) {
        j["message"] = pauseRequest.getMessage();
    }

    void from_json(const nlohmann::json &j, PauseRequest &pauseRequest) {
        pauseRequest = PauseRequest{
            j.at("message").get<std::string>()
        };
    }
}
