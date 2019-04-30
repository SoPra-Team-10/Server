/**
 * @file PauseResponse.hpp
 * @author paul
 * @date 19.03.19
 * @brief Declaration of the class PauseResponse
 */

#ifndef SERVER_PAUSERESPONSE_HPP
#define SERVER_PAUSERESPONSE_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class PauseResponse {
    public:
        PauseResponse() = default;
        PauseResponse(std::string message, std::string userName, bool pause);

        std::string getMessage() const;
        std::string getUserName() const;
        bool isPause() const;

        static auto getName() -> std::string;

        bool operator==(const PauseResponse &rhs) const;

        bool operator!=(const PauseResponse &rhs) const;

    private:
        std::string message, userName;
        bool pause;
    };

    void to_json(nlohmann::json &j, const PauseResponse &pauseResponse);
    void from_json(const nlohmann::json &j, PauseResponse &pauseResponse);
}

#endif //SERVER_PAUSERESPONSE_HPP
