#include <utility>

/**
 * @file ContinueRequest.cpp
 * @author paul
 * @date 19.03.19
 * @brief ContinueRequest @TODO
 */

#include "ContinueRequest.hpp"

namespace communication::messages::request {

    auto ContinueRequest::getName() -> std::string {
        return "continueRequest";
    }

    ContinueRequest::ContinueRequest(std::string message) : message(std::move(message)) {}

    std::string ContinueRequest::getMessage() const {
        return message;
    }

    void to_json(nlohmann::json &j, const ContinueRequest &continueRequest) {
        j["message"] = continueRequest.getMessage();
    }

    void from_json(const nlohmann::json &j, ContinueRequest &continueRequest) {
        continueRequest = ContinueRequest{
                j.at("message").get<std::string>()
        };
    }
}
