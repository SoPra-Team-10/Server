/**
 * @file JoinRequest.cpp
 * @author paul
 * @date 19.03.19
 * @brief JoinRequest @TODO
 */

#include "JoinRequest.hpp"

namespace communication::messages::request {

    auto JoinRequest::getName() -> std::string {
        return "joinRequest";
    }
}
