//
// Created by paulnykiel on 04.05.19.
//

#ifndef SERVER_COMMUNICATORTEST_H
#define SERVER_COMMUNICATORTEST_H

#include <src/Communication/Communicator.hpp>

namespace communication {
    class CommunicatorTest : public Communicator {
    public:
        using Communicator::Communicator;
        void receiveTest(messages::Message message, int client) {
            this->receive(message, client);
        }
    };
}

#endif //SERVER_COMMUNICATORTEST_H
