/**
 * @file AnimationQueue.cpp
 * @author paul
 * @date 07.05.19
 * @brief Implementation of the AnimationQueue class
 */

#include "AnimationQueue.h"

#include "Communicator.hpp"

namespace communication {
    AnimationQueue::AnimationQueue(Communicator &communicator)
        : communicator{communicator}, finished{false} {
        this->threadHandler = std::async(std::launch::async, std::bind(&AnimationQueue::run, this));
    }

    void AnimationQueue::add(const messages::Payload& payload, const std::vector<int>& clients, std::chrono::milliseconds timeOffset) {
        std::lock_guard<std::mutex> lock{mapLock};
        std::chrono::milliseconds toSendTime{0};
        if (!toSend.empty()) {
            toSendTime = (--toSend.end())->first;
        }

        if (std::holds_alternative<messages::broadcast::Snapshot>(payload)) {
            auto &snapshot = std::get<messages::broadcast::Snapshot>(payload);
            if (lastSnapshotForType.find(snapshot.getPhase()) != lastSnapshotForType.end()) {
                if (lastSnapshotForType.at(snapshot.getPhase()) + timeOffset > toSendTime) {
                    toSendTime = lastSnapshotForType.at(snapshot.getPhase()) + timeOffset;
                }
            }
            for (const auto &msg : toSend) {
                if (std::holds_alternative<messages::broadcast::Snapshot>(msg.second.first)) {
                    auto &otherSnapshot = std::get<messages::broadcast::Snapshot>(msg.second.first);
                    if (otherSnapshot.getPhase() == snapshot.getPhase() && msg.first + timeOffset > toSendTime) {
                        toSendTime = msg.first + timeOffset;
                    }
                }
            }
        }
        // Keep the elements unique, if there is not enough time to the next message we have a problem
        while (toSend.find(toSendTime) != toSend.end()) {
            ++toSendTime;
        }
        auto [newIt,tookPlace] = toSend.emplace(toSendTime, std::make_pair(payload, clients));
        if(newIt == toSend.begin()) {
            cv.notify_all();
        }
    }

    void AnimationQueue::run() {
        while (!finished) {
            std::unique_lock<std::mutex> lock{delayLock};
            if (toSend.empty()) {
                cv.wait(lock);
            } else {
                cv.wait_until(lock, std::chrono::system_clock::time_point{toSend.begin()->first});
            }
            auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch());
            mapLock.lock();
            while (!toSend.empty() && now >= toSend.begin()->first) {
                auto msgInfo = toSend.begin()->second;
                for (const auto &id : msgInfo.second) {
                    communicator.send(messages::Message{msgInfo.first}, id);
                }
                toSend.erase(toSend.begin());
            }
            mapLock.unlock();
        }
    }

    AnimationQueue::~AnimationQueue() {
        this->finished = true;
        cv.notify_all();
        this->threadHandler.wait();

        // Send all remaining messages now
        for (const auto &msg : toSend) {
            for (const auto &clients : msg.second.second) {
                communicator.send(messages::Message{msg.second.first}, clients);
            }
        }
    }
}
