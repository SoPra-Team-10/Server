/**
 * @file MatchConfig.cpp
 * @author paul
 * @date 19.03.19
 * @brief MatchConfig @TODO
 */

#include "MatchConfig.hpp"

namespace communication::messages::broadcast {

    auto MatchConfig::getName() -> std::string {
        return "matchConfig";
    }
}
