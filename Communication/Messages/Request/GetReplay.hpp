/**
 * @file GetReplay.hpp
 * @author paul
 * @date 19.03.19
 * @brief GetReplay @TODO
 */

#ifndef SERVER_GETREPLAY_HPP
#define SERVER_GETREPLAY_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::request {
    class GetReplay {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_GETREPLAY_HPP
