/**
 * @file PauseResponse.hpp
 * @author paul
 * @date 19.03.19
 * @brief PauseResponse @TODO
 */

#ifndef SERVER_PAUSERESPONSE_HPP
#define SERVER_PAUSERESPONSE_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class PauseResponse {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_PAUSERESPONSE_HPP
