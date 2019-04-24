/**
 * @file MatchStart.hpp
 * @author paul
 * @date 19.03.19
 * @brief MatchStart @TODO
 */

#ifndef SERVER_MATCHSTART_HPP
#define SERVER_MATCHSTART_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class MatchStart {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_MATCHSTART_HPP
