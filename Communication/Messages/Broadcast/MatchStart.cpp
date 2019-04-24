/**
 * @file MatchStart.cpp
 * @author paul
 * @date 19.03.19
 * @brief MatchStart @TODO
 */

#include "MatchStart.hpp"

namespace communication::messages::broadcast {

    auto MatchStart::getName() -> std::string {
        return "matchStart";
    }
}
