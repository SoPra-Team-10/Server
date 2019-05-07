//
// Created by paulnykiel on 07.05.19.
//

#include "AnimationQueue.h"

#include "Communicator.hpp"

namespace communication {
    AnimationQueue::AnimationQueue(Communicator &communicator)
        : communicator{communicator}, finished{false} {
        this->threadHandler = std::async(std::launch::async, std::bind(&AnimationQueue::run, this));
    }

    void AnimationQueue::add(const messages::Payload& payload, const std::vector<int>& clients, std::chrono::milliseconds timeOffset) {
        std::lock_guard<std::mutex> lock{mapLock};
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        std::chrono::milliseconds lastMessageTime{0};
        if (std::holds_alternative<messages::broadcast::Snapshot>(payload)) {
            auto &snapshot = std::get<messages::broadcast::Snapshot>(payload);
            if (lastSnapshotForType.find(snapshot.getPhase()) != lastSnapshotForType.end()) {
                lastMessageTime = lastSnapshotForType.at(snapshot.getPhase());
            }
            for (const auto &msg : toSend) {
                if (std::holds_alternative<messages::broadcast::Snapshot>(msg.second.first)) {
                    auto &otherSnapshot = std::get<messages::broadcast::Snapshot>(msg.second.first);
                    if (otherSnapshot.getPhase() == snapshot.getPhase() && msg.first > lastMessageTime) {
                        lastMessageTime = msg.first;
                    }
                }
            }
        } else {
            lastMessageTime = now;
        }
        auto toCallTime = lastMessageTime + timeOffset;
        auto [newIt,tookPlace] = toSend.emplace(toCallTime, std::make_pair(payload, clients));
        if(newIt == toSend.begin()) {
            cv.notify_all();
        }
    }

    void AnimationQueue::run() {
        while (!finished) {
            auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch());
            mapLock.lock();
            while (!toSend.empty() && now > toSend.begin()->first) {
                auto msgInfo = toSend.begin()->second;
                for (const auto &id : msgInfo.second) {
                    communicator.send(messages::Message{msgInfo.first}, id);
                }
                toSend.erase(toSend.begin());
            }
            mapLock.unlock();

            std::unique_lock<std::mutex> lock{delayLock};
            if (toSend.empty()) {
                cv.wait(lock);
            } else {
                cv.wait_until(lock, std::chrono::system_clock::time_point{toSend.begin()->first});
            }
        }
    }

    AnimationQueue::~AnimationQueue() {
        this->finished = true;
        this->threadHandler.wait();
    }
}
