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

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_JOINRESPONSE_HPP
