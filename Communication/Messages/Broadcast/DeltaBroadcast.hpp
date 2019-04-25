/**
 * @file DeltaBroadcast.hpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaBroadcast @TODO
 */

#ifndef SERVER_DELTABROADCAST_HPP
#define SERVER_DELTABROADCAST_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class DeltaBroadcast {
    public:

        static auto getName() -> std::string;
    private:
    };
}

#endif //SERVER_DELTABROADCAST_HPP
