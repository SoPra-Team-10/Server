/**
 * @file MatchConfig.hpp
 * @author paul
 * @date 19.03.19
 * @brief MatchConfig @TODO
 */

#ifndef SERVER_MATCHCONFIG_HPP
#define SERVER_MATCHCONFIG_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class MatchConfig {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_MATCHCONFIG_HPP
