//
// Created by bjorn on 04.05.19.
//

#ifndef SERVER_TIMER_H
#define SERVER_TIMER_H


#include <thread>

class Timer {
    bool clear = false;

public:
    /**
     *
     * @param delay after the delay, the code will be execute
     */
    template<typename Function>
    void setTimeout(Function function, int delay);

    /**
     * @param interval in every distance of the interval, the code will be execute
     */
    template<typename Function>
    void setInterval(Function function, int interval);

    void stop();
};

template<typename Function>
void Timer::setTimeout(Function function, int delay) {
    this->clear = false;
    std::thread t([=]() {
        if(this->clear) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(this->clear) return;
        function();
    });
    t.detach();
}

template<typename Function>
void Timer::setInterval(Function function, int interval) {
    this->clear = false;
    std::thread t([=]() {
        while(true) {
            if(this->clear)
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(this->clear) return;
            function();
        }
    });
    t.detach();
}


#endif //SERVER_TIMER_H
