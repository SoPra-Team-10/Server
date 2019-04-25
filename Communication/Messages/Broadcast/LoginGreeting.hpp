/**
 * @file LoginGreeting.hpp
 * @author paul
 * @date 19.03.19
 * @brief LoginGreeting @TODO
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
    private:
        std::string userName;
    };
}

#endif //SERVER_LOGINGREETING_HPP
