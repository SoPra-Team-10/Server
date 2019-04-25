/**
 * @file Reconnect.hpp
 * @author paul
 * @date 19.03.19
 * @brief Reconnect @TODO
 */

#ifndef SERVER_RECONNECT_HPP
#define SERVER_RECONNECT_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::unicast {
    class Reconnect {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_RECONNECT_HPP
