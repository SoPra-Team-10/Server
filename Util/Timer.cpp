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
}
