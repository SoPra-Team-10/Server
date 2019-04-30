#include <utility>

/**
 * @file PrivateDebug.cpp
 * @author paul
 * @date 19.03.19
 * @brief Declaration of the PrivateDebug class
 */

#include "PrivateDebug.hpp"

namespace communication::messages::unicast {

    PrivateDebug::PrivateDebug(std::string information) : information{std::move(information)} {}

    auto PrivateDebug::getInformation() const -> std::string {
        return this->information;
    }

    auto PrivateDebug::getName() -> std::string {
        return "privateDebug";
    }

    void PrivateDebug::setInformation(const std::string &information) {
        this->information = information;
    }

    bool PrivateDebug::operator==(const PrivateDebug &rhs) const {
        return information == rhs.information;
    }

    bool PrivateDebug::operator!=(const PrivateDebug &rhs) const {
        return !(rhs == *this);
    }

    void to_json(nlohmann::json &j, const PrivateDebug &privateDebug) {
        j = nlohmann::json{{"information", privateDebug.getInformation()}};
    }

    void from_json(const nlohmann::json &j, PrivateDebug &privateDebug) {
        privateDebug = PrivateDebug{j.at("information").get<std::string>()};
    }
}
