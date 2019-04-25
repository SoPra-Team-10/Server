/**
 * @file DeltaRequest.cpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaRequest @TODO
 */

#include "DeltaRequest.hpp"

namespace communication::messages::request {

    auto DeltaRequest::getName() -> std::string {
        return "deltaRequest";
    }
}
