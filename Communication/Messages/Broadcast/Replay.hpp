/**
 * @file Replay.hpp
 * @author paul
 * @date 19.03.19
 * @brief Replay @TODO
 */

#ifndef SERVER_REPLAY_HPP
#define SERVER_REPLAY_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class Replay {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_REPLAY_HPP
