/**
 * @file JoinResponse.hpp
 * @author paul
 * @date 19.03.19
 * @brief JoinResponse @TODO
 */

#ifndef SERVER_JOINRESPONSE_HPP
#define SERVER_JOINRESPONSE_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::unicast {
    class JoinResponse {
    public:
        JoinResponse() = default;
        explicit JoinResponse(std::string message);
        auto getMessage() const -> std::string;

        static auto getName() -> std::string;
    private:
        std::string message;
    };
}

#endif //SERVER_JOINRESPONSE_HPP
