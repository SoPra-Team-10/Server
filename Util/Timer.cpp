//
// Created by bjorn on 04.05.19.
//

#include "Timer.h"

namespace util {
    void Timer::pause() {
        paused = true;
    }

    void Timer::resume() {
        paused = false;
    }

    void Timer::stop() {
        stopRequired = true;
    }

    Timer::~Timer() {
        if (threadHandler.valid()) {
            this->stop();
            this->threadHandler.wait();
        }
    }
}
