/**
 * @file DeltaRequest.hpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaRequest @TODO
 */

#ifndef SERVER_DELTAREQUEST_HPP
#define SERVER_DELTAREQUEST_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::request {
    class DeltaRequest {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_DELTAREQUEST_HPP
