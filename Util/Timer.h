//
// Created by bjorn on 04.05.19.
//

#ifndef SERVER_TIMER_H
#define SERVER_TIMER_H


#include <thread>
#include <future>
#include <atomic>
#include <cmath>

#define RESOLUTION 10

namespace util {
    class Timer {
    public:
        /**
         *
         * @param delay after the delay, the code will be execute
         */
        template<typename Function>
        void setTimeout(Function function, int delay);

        /**
         * Pauses the Timer
         */
        void pause();

        /**
         * Resumes the timer
         */
        void resume();

        /**
         * Stops the timer without calling the event
         */
        void stop();

        ~Timer();
    private:
        std::atomic_bool paused{false};
        std::atomic_bool stopRequired{false};
        int steps = 0;
        std::future<void> threadHandler;
    };

    template<typename Function>
    void Timer::setTimeout(Function function, int delay) {
        static_assert(RESOLUTION <= 1000, "Time resolution must not be greater than 1000");
        if (steps > 0) {
            throw std::runtime_error{"A timer can only manage a single event!"};
        }
        stopRequired = false;
        paused = false;
        steps = static_cast<int>(std::ceil(delay * (1000.0 / RESOLUTION)));
        threadHandler = std::async(std::launch::async, [=](){
            std::this_thread::sleep_for(std::chrono::milliseconds(RESOLUTION));
            if (stopRequired) {
                return;
            } else if (!paused) {
                if (--steps <= 0) {
                    function();
                    return;
                }
            }
        });
    }
}

#endif //SERVER_TIMER_H
