/**
 * @file GlobalDebug.cpp
 * @author paul
 * @date 19.03.19
 * @brief Definition of the GlobalDebug class
 */

#include "GlobalDebug.hpp"

namespace communication::messages::broadcast {

    GlobalDebug::GlobalDebug(std::string information) : information{std::move(information)} {}

    auto GlobalDebug::getInformation() const -> std::string {
        return this->information;
    }

    auto GlobalDebug::getName() -> std::string {
        return "globalDebug";
    }

    bool GlobalDebug::operator==(const GlobalDebug &rhs) const {
        return information == rhs.information;
    }

    bool GlobalDebug::operator!=(const GlobalDebug &rhs) const {
        return !(rhs == *this);
    }

    void to_json(nlohmann::json &j, const GlobalDebug &globalDebug) {
        j = nlohmann::json{{"information", globalDebug.getInformation()}};
    }

    void from_json(const nlohmann::json &j, GlobalDebug &globalDebug) {
        globalDebug = GlobalDebug{j.at("information").get<std::string>()};
    }
}
