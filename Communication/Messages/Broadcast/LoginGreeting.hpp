/**
 * @file LoginGreeting.hpp
 * @author paul
 * @date 19.03.19
 * @brief Declaration of the LoginGreeting class
 */

#ifndef SERVER_LOGINGREETING_HPP
#define SERVER_LOGINGREETING_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class LoginGreeting {
    public:
        LoginGreeting() = default;
        explicit LoginGreeting(std::string userName);
        auto getUserName() const -> std::string;

        static auto getName() -> std::string;

        bool operator==(const LoginGreeting &rhs) const;

        bool operator!=(const LoginGreeting &rhs) const;

    private:
        std::string userName;
    };

    void to_json(nlohmann::json &j, const LoginGreeting &loginGreeting);
    void from_json(const nlohmann::json &j, LoginGreeting &loginGreeting);
}

#endif //SERVER_LOGINGREETING_HPP
