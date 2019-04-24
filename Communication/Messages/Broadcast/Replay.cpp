/**
 * @file Replay.cpp
 * @author paul
 * @date 19.03.19
 * @brief Replay @TODO
 */

#include "Replay.hpp"

namespace communication::messages::broadcast {

    auto Replay::getName() -> std::string {
        return "replay";
    }
}
