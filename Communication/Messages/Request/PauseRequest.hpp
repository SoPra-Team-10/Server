/**
 * @file PauseRequest.hpp
 * @author paul
 * @date 19.03.19
 * @brief PauseRequest @TODO
 */

#ifndef SERVER_PAUSEREQUEST_HPP
#define SERVER_PAUSEREQUEST_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::request {
    class PauseRequest {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_PAUSEREQUEST_HPP
