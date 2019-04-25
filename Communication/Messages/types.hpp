/**
 * @file types.hpp
 * @author paul
 * @date 25.04.19
 * @brief types @TODO
 */

#ifndef SERVER_TYPES_HPP
#define SERVER_TYPES_HPP

namespace communication::messages::types {
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
    enum class Sex {
        M, F
    };
}

#endif //SERVER_TYPES_HPP
