/**
 * @file Next.hpp
 * @author paul
 * @date 19.03.19
 * @brief Next @TODO
 */

#ifndef SERVER_NEXT_HPP
#define SERVER_NEXT_HPP

#include <string>
#include <Lib/json.hpp>

#include "Communication/Messages/Common/types.hpp"

namespace communication::messages::broadcast {
    class Next {
    public:
        Next() = default;
        Next(types::EntityId entityId, types::TurnType turnType, std::string timout);

        types::EntityId getEntityId() const;

        types::TurnType getTurnType() const;

        std::string getTimout() const;

        static auto getName() -> std::string;
    private:
        types::EntityId entityId;
        types::TurnType turnType;
        std::string timout; //@TODO don't know if this is really correct, but that's what is written in the spec
    };

    void to_json(nlohmann::json &j, const Next &next);
    void from_json(const nlohmann::json &j, Next &next);
}

#endif //SERVER_NEXT_HPP
