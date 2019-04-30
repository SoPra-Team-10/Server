/**
 * @file ContinueRequest.hpp
 * @author paul
 * @date 19.03.19
 * @brief Definition of the ContinueRequest class
 */

#ifndef SERVER_CONTINUEREQUEST_HPP
#define SERVER_CONTINUEREQUEST_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::request {
    class ContinueRequest {
    public:
        ContinueRequest() = default;
        explicit ContinueRequest(std::string message);

        std::string getMessage() const;

        static auto getName() -> std::string;

        bool operator==(const ContinueRequest &rhs) const;

        bool operator!=(const ContinueRequest &rhs) const;

    private:
        std::string message;
    };

    void to_json(nlohmann::json &j, const ContinueRequest &continueRequest);
    void from_json(const nlohmann::json &j, ContinueRequest &continueRequest);
}

#endif //SERVER_CONTINUEREQUEST_HPP
