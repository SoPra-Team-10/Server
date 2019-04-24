/**
 * @file SendDebug.hpp
 * @author paul
 * @date 19.03.19
 * @brief SendDebug @TODO
 */

#ifndef SERVER_SENDDEBUG_HPP
#define SERVER_SENDDEBUG_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::request {
    class SendDebug {
    public:
        SendDebug() = default;
        SendDebug(std::string information);
        auto getInformation() const -> std::string;

        static auto getName() -> std::string;

    private:
        std::string information;
    };

    void to_json(nlohmann::json &j, const SendDebug &sendDebug);
    void from_json(const nlohmann::json &j, SendDebug &sendDebug);
}

#endif //SERVER_SENDDEBUG_HPP
