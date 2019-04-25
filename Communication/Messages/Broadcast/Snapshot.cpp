/**
 * @file Snapshot.cpp
 * @author paul
 * @date 19.03.19
 * @brief Snapshot @TODO
 */

#include "Snapshot.hpp"

namespace communication::messages::broadcast {

    auto Snapshot::getName() -> std::string {
        return "snapshot";
    }
}
