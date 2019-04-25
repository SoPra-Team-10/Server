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
        PauseRequest(std::string message);

        std::string getMessage() const;

        static auto getName() -> std::string;
    private:
        std::string message;
    };

    void to_json(nlohmann::json &j, const PauseRequest &pauseRequest);
    void from_json(const nlohmann::json &j, PauseRequest &pauseRequest);
}

#endif //SERVER_PAUSEREQUEST_HPP
