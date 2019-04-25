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
                bool isArtificialIntelligence = false, std::vector<std::string> mods = {});
        auto getLobby() const -> std::string;
        auto getUserName() const -> std::string;
        auto getPassword() const -> std::string;
        auto getIsAi() const -> bool;
        auto getMods() const -> std::vector<std::string>;

        static auto getName() -> std::string;
    private:
        std::string lobby, userName, password; // Very secure, much hash
        bool isArtificialIntelligence;
        std::vector<std::string> mods; //@TODO add to enum
    };

    void to_json(nlohmann::json &j, const JoinRequest &joinRequest);
    void from_json(const nlohmann::json &j, JoinRequest &joinRequest);
}

#endif //SERVER_JOINREQUEST_HPP
