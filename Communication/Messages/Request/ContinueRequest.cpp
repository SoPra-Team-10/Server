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
}
