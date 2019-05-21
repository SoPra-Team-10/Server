//
// Created by bjorn on 04.05.19.
//

#ifndef SERVER_TIMER_H
#define SERVER_TIMER_H


#include <thread>
#include <future>
#include <atomic>
#include <variant>

#define RESOLUTION 100

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

        /**
         * DTor: Necessary for cleanup and stopping of the thread
         */
        ~Timer();

        using Timepoint = std::chrono::time_point<std::chrono::system_clock>;
        using Duration = std::chrono::milliseconds;
    private:
        std::atomic_bool stopRequired{false};
        std::condition_variable conditionVariable;
        std::future<void> threadHandler;
        std::future<void> functionThreadHandler;
        std::mutex mutex;
        std::variant<Timepoint, Duration> time;
    };

    template<typename Function>
    void Timer::setTimeout(Function function, int delay) {
        stopRequired = false;
        time = std::chrono::system_clock::now() + std::chrono::milliseconds{delay};

        threadHandler = std::async(std::launch::async, [=](){
            while (!stopRequired) {
                std::unique_lock<std::mutex> lock{mutex};
                if (std::holds_alternative<Timepoint>(time)) { // Running
                    auto now = std::chrono::system_clock::now();
                    auto timepoint = std::get<Timepoint>(time);
                    if (now >= timepoint) {
                        functionThreadHandler = std::async(std::launch::async, function);
                        return;
                    }
                    conditionVariable.wait_until(lock, timepoint);
                } else { // Pause
                    conditionVariable.wait(lock);
                }
            }
        });
    }
}

#endif //SERVER_TIMER_H
