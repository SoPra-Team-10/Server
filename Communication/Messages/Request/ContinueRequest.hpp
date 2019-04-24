/**
 * @file ContinueRequest.hpp
 * @author paul
 * @date 19.03.19
 * @brief ContinueRequest @TODO
 */

#ifndef SERVER_CONTINUEREQUEST_HPP
#define SERVER_CONTINUEREQUEST_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::request {
    class ContinueRequest {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_CONTINUEREQUEST_HPP
