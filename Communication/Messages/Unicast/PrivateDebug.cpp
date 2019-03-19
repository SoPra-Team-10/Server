/**
 * @file PrivateDebug.cpp
 * @author paul
 * @date 19.03.19
 * @brief PrivateDebug @TODO
 */

#include "PrivateDebug.hpp"

namespace communication::messages::unicast {

    PrivateDebug::PrivateDebug(const std::string &information) : information{information} {}

    auto PrivateDebug::getInformation() const -> std::string {
        return this->information;
    }

    auto PrivateDebug::getName() -> std::string {
        return "privateDebug";
    }

    void to_json(nlohmann::json &j, const PrivateDebug &privateDebug) {
        j = nlohmann::json{{"information", privateDebug.getInformation()}};
    }

    void from_json(const nlohmann::json &j, PrivateDebug &privateDebug) {
        privateDebug = PrivateDebug{j.at("information").get<std::string>()};
    }
}
