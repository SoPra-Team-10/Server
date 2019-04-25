/**
 * @file DeltaBroadcast.cpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaBroadcast @TODO
 */

#include "DeltaBroadcast.hpp"

namespace communication::messages::broadcast {

    auto DeltaBroadcast::getName() -> std::string {
        return "deltaBroadcast";
    }
}
