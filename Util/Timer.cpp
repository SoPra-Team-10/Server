//
// Created by bjorn on 04.05.19.
//

#include "Timer.h"

namespace util {
    void Timer::pause() {
        if (std::holds_alternative<Timepoint>(time)) { // Check if we are actually running
            auto timepoint = std::get<Timepoint>(time);
            auto now = std::chrono::system_clock::now();
            auto duration = timepoint - now;
            time = std::chrono::duration_cast<Duration>(duration);
            conditionVariable.notify_all();
        }
    }

    void Timer::resume() {
        if (std::holds_alternative<Duration>(time)) { // Check if pause
            auto duration = std::get<Duration>(time);
            auto now = std::chrono::system_clock::now();
            auto timepoint = now + duration;
            time = timepoint;
            conditionVariable.notify_all();
        }
    }

    void Timer::stop() {
        stopRequired = true;
        conditionVariable.notify_all();
        if(threadHandler.valid()){
            threadHandler.get();
        }
    }

    Timer::~Timer() {
        if (threadHandler.valid()) {
            this->stop();
        }

        if (functionThreadHandler.valid()) {
            functionThreadHandler.get();
        }
    }
}
