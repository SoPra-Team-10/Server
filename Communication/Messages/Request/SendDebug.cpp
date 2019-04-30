#include <utility>

/**
 * @file SendDebug.cpp
 * @author paul
 * @date 19.03.19
 * @brief SendDebug @TODO
 */

#include "SendDebug.hpp"

namespace communication::messages::request {

    SendDebug::SendDebug(std::string information) : information{std::move(information)} {}

    auto SendDebug::getInformation() const -> std::string {
        return this->information;
    }

    auto SendDebug::getName() -> std::string {
        return "sendDebug";
    }

    bool SendDebug::operator==(const SendDebug &rhs) const {
        return information == rhs.information;
    }

    bool SendDebug::operator!=(const SendDebug &rhs) const {
        return !(rhs == *this);
    }

    void to_json(nlohmann::json &j, const SendDebug &sendDebug) {
        j = nlohmann::json{{"information", sendDebug.getInformation()}};
    }

    void from_json(const nlohmann::json &j, SendDebug &sendDebug) {
        sendDebug = SendDebug{j.at("information").get<std::string>()};
    }
}
