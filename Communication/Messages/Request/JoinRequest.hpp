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
        JoinRequest() = default;
        JoinRequest(std::string lobby, std::string userName, std::string password,
                bool isArtificialIntelligence = false, std::optional<std::string> mods = std::nullopt);
        auto getLobby() const -> std::string;
        auto getUserName() const -> std::string;
        auto getPassword() const -> std::string;
        auto getIsAi() const -> bool;
        auto getMods() const -> std::optional<std::string>;

        static auto getName() -> std::string;
    private:
        std::string lobby, userName, password; // Very secure, much hash
        bool isArtificialIntelligence;
        std::optional<std::string> mods;
    };
}

#endif //SERVER_JOINREQUEST_HPP
