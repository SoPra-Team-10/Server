/**
 * @file types.hpp
 * @author paul
 * @date 25.04.19
 * @brief types @TODO
 */

#ifndef SERVER_TYPES_HPP
#define SERVER_TYPES_HPP

#include <string>

namespace communication::messages::types {
    enum class EntityId {
        LEFT_SEEKER, LEFT_KEEPER, LEFT_CHASER1, LEFT_CHASER2, LEFT_CHASER3, LEFT_BEATER1, LEFT_BEATER2,
        RIGHT_SEEKER, RIGHT_KEEPER, RIGHT_CHASER1, RIGHT_CHASER2, RIGHT_CHASER3, RIGHT_BEATER1, RIGHT_BEATER2,
        SNITCH, BLUDGER1, BLUDGER2, QUAFFLE,
        LEFT_GOBLIN, LEFT_TROLL, LEFT_ELF, LEFT_NIFFLER,
        RIGHT_GOBLIN, RIGHT_TROLL, RIGHT_ELF, RIGHT_NIFFLER
    };

    auto toString(EntityId entityId) -> std::string;
    auto fromStringEntityId(const std::string &s) -> EntityId;

    enum class TurnType {
        MOVE, ACTION
    };

    auto toString(TurnType turnType) -> std::string;
    auto fromStringTurnType(const std::string &s) -> TurnType;

    enum class VictoryReason {
        DISQUALIFICATION, BOTH_DISQUALIFICATION_MOST_POINTS,
        BOTH_DISQUALIFICATION_POINTS_EQUAL_SNITCH_CATCH,
        BOTH_DISQUALIFICATION_POINTS_EQUAL_LAST_DISQUALIFICATION,
        MOST_POINTS,
        POINTS_EQUAL_SNITCH_CATCH,
        VIOLATION_OF_PROTOCOL
    };

    enum class FanType {
        GOBLIN, TROLL, ELF, NIFFLER
    };

    enum class Broom {
        THINDERBLAST, CLEANSWEEP11, COMET260, NIMBUS2001, FIREBOLT
    };

    auto toString(Broom broom) -> std::string;
    auto fromStringBroom(const std::string &s) -> Broom;

    enum class Sex {
        M, F
    };

    auto toString(Sex s) -> std::string;
    auto fromStringSex(const std::string &s) -> Sex;

    enum class DeltaType {
        SNITCH_CATCH, BLUDGER_BEATING, QUAFFLE_THROW,
        SNITCH_SNATCH, TROLL_ROAR, ELF_TELEPORTATION,
        GOBLIN_SHOCK, BAN, BLUDGER_KNOCKOUT, MOVE
    };
}

#endif //SERVER_TYPES_HPP
