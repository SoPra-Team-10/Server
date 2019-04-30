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
        Next(types::EntityId entityId, types::TurnType turnType, int timout);

        types::EntityId getEntityId() const;

        types::TurnType getTurnType() const;

        int getTimout() const;

        static auto getName() -> std::string;

        bool operator==(const Next &rhs) const;

        bool operator!=(const Next &rhs) const;

    private:
        types::EntityId entityId;
        types::TurnType turnType;
        int timeout;
    };

    void to_json(nlohmann::json &j, const Next &next);
    void from_json(const nlohmann::json &j, Next &next);
}

#endif //SERVER_NEXT_HPP
