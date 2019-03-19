/**
 * @file Communicator.cpp
 * @author paul
 * @date 15.03.19
 * @brief Communicator @TODO
 */

#include "Communicator.hpp"

namespace communication {

    Communicator::Communicator(uint16_t port) :
        webSocketServer{port, "test"} {

    }
}