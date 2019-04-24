/**
 * @file TeamFormation.cpp
 * @author paul
 * @date 19.03.19
 * @brief TeamFormation @TODO
 */

#include "TeamFormation.hpp"

namespace communication::messages::request {

    auto TeamFormation::getName() -> std::string {
        return "teamFormation";
    }
}
