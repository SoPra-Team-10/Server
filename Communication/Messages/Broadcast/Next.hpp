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

namespace communication::messages::broadcast {
    class Next {
    public:
        enum class EntityId {
            LEFT_SEEKER, LEFT_KEEPER, LEFT_CHASER1, LEFT_CHASER2, LEFT_CHASER3, LEFT_BEATER1, LEFT_BEATER2,
            RIGHT_SEEKER, RIGHT_KEEPER, RIGHT_CHASER1, RIGHT_CHASER2, RIGHT_CHASER3, RIGHT_BEATER1, RIGHT_BEATER2,
            SNITCH, BLUDGER1, BLUDGER2, QUAFFLE,
            LEFT_GOBLIN, LEFT_TROLL, LEFT_ELF, LEFT_NIFFLER,
            RIGHT_GOBLIN, RIGHT_TROLL, RIGHT_ELF, RIGHT_NIFFLER
        };

        enum class TurnType {
            MOVE, ACTION
        };

        Next(EntityId entityId, TurnType turnType, std::string timout);

        EntityId getEntityId() const;

        TurnType getTurnType() const;

        std::string getTimout() const;

        static auto getName() -> std::string;
    private:
        EntityId entityId;
        TurnType turnType;
        std::string timout; //@TODO don't know if this is really correct, but that's what is written in the spec
    };

    void to_json(nlohmann::json &j, const Next &next);
    void from_json(const nlohmann::json &j, Next &next);
}

#endif //SERVER_NEXT_HPP
