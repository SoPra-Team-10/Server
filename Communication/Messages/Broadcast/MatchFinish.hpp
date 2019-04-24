/**
 * @file MatchFinish.hpp
 * @author paul
 * @date 19.03.19
 * @brief MatchFinish @TODO
 */

#ifndef SERVER_MATCHFINISH_HPP
#define SERVER_MATCHFINISH_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class MatchFinish {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_MATCHFINISH_HPP
