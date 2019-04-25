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
}
