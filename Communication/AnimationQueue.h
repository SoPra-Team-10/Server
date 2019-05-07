//
// Created by paulnykiel on 07.05.19.
//

#ifndef SERVER_ANIMATIONQUEUE_H
#define SERVER_ANIMATIONQUEUE_H

#include <map>
#include <chrono>
#include <atomic>
#include <future>
#include <SopraMessages/Message.hpp>

namespace communication {
    class Communicator;

    class AnimationQueue {
    public:
        explicit AnimationQueue(Communicator &communicator);
        void add(const messages::Payload& payload, const std::vector<int>& clients, std::chrono::milliseconds timeOffset = std::chrono::milliseconds{0});
        ~AnimationQueue();
    private:
        void run();

        Communicator &communicator;
        std::map<std::chrono::milliseconds, std::pair<messages::Payload, std::vector<int>>> toSend;
        std::map<messages::types::PhaseType, std::chrono::milliseconds> lastSnapshotForType;
        std::future<void> threadHandler;
        std::atomic_bool finished;
        std::mutex mapLock;
        std::mutex delayLock;
        std::condition_variable cv;
    };
}

#endif //SERVER_ANIMATIONQUEUE_H
