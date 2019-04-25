#include <utility>

#include <utility>

/**
 * @file PauseResponse.cpp
 * @author paul
 * @date 19.03.19
 * @brief PauseResponse @TODO
 */

#include "PauseResponse.hpp"

namespace communication::messages::broadcast {

    auto PauseResponse::getName() -> std::string {
        return "pauseResponse";
    }

    PauseResponse::PauseResponse(std::string message, std::string userName, bool pause) : message(std::move(
            message)), userName(std::move(userName)), pause(pause) {}

    std::string PauseResponse::getMessage() const {
        return message;
    }

    std::string PauseResponse::getUserName() const {
        return userName;
    }

    bool PauseResponse::isPause() const {
        return pause;
    }

    void to_json(nlohmann::json &j, const PauseResponse &pauseResponse) {
        j["message"] = pauseResponse.getMessage();
        j["userName"] = pauseResponse.getUserName();
        j["pause"] = pauseResponse.isPause();
    }

    void from_json(const nlohmann::json &j, PauseResponse &pauseResponse) {
        pauseResponse = PauseResponse{
            j.at("message").get<std::string>(),
            j.at("userName").get<std::string>(),
            j.at("pause").get<bool>()
        };
    }
}
