/**
 * @file JoinRequest.hpp
 * @author paul
 * @date 19.03.19
 * @brief JoinRequest @TODO
 */

#ifndef SERVER_JOINREQUEST_HPP
#define SERVER_JOINREQUEST_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::request {
    class JoinRequest {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_JOINREQUEST_HPP
