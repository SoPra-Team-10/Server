/**
 * @file AnimationQueue.h
 * @author paul
 * @date 07.05.19
 * @brief Declaration of the AnimationQueue class
 */

#ifndef SERVER_ANIMATIONQUEUE_H
#define SERVER_ANIMATIONQUEUE_H

#include <map>
#include <chrono>
#include <atomic>
#include <future>
#include <SopraMessages/Message.hpp>

namespace communication {
    class Communicator;

    /**
     * The AnimationQueue is responsible for delaying messages to preserve the animation duration while
     * also keeping the order of the message
     */
    class AnimationQueue {
    public:
        /**
         * CTor: Construct the AnimationQueue with a communicator to which to send
         * @param communicator reference to the communicator to which to send
         */
        explicit AnimationQueue(Communicator &communicator);

        /**
         * Add an payload to the queue to send
         * @param payload the payload to send
         * @param clients list off all clients to which to send the message
         * @param timeOffset the time between: the current time if the queue is empty, at the end of the queue
         *      if the queue is non empty but all offsets are obeyed or some time after the queue to obey the
         *      offset (even if the queue is empty it is checked with the last sent message)
         */
        void add(const messages::Payload& payload, const std::vector<int>& clients, std::chrono::milliseconds timeOffset = std::chrono::milliseconds{0});

        /**
         * DTor: Stops the thread
         */
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
