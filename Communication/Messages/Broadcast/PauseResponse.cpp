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
}
