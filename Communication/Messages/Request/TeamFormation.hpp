/**
 * @file TeamFormation.hpp
 * @author paul
 * @date 19.03.19
 * @brief TeamFormation @TODO
 */

#ifndef SERVER_TEAMFORMATION_HPP
#define SERVER_TEAMFORMATION_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::request {
    class TeamFormation {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_TEAMFORMATION_HPP
