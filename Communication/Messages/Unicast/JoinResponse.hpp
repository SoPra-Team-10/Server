/**
 * @file JoinResponse.hpp
 * @author paul
 * @date 19.03.19
 * @brief Declaration of the JoinResponse class
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

        bool operator==(const JoinResponse &rhs) const;

        bool operator!=(const JoinResponse &rhs) const;

    private:
        std::string message;
    };

    void to_json(nlohmann::json &j, const JoinResponse &joinResponse);
    void from_json(const nlohmann::json &j, JoinResponse &joinResponse);
}

#endif //SERVER_JOINRESPONSE_HPP
