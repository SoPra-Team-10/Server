//
// Created by timluchterhand on 07.05.19.
//

#ifndef SERVER_CONVERSIONS_H
#define SERVER_CONVERSIONS_H

#include <SopraGameLogic/GameModel.h>
#include "GameTypes.h"

namespace conversions{
    auto foulToBanReason(gameModel::Foul foul) -> communication::messages::types::BanReason;
    auto interferenceToId(gameModel::InterferenceType type, gameHandling::TeamSide side) -> communication::messages::types::EntityId;
    bool isFan(communication::messages::types::EntityId id);
    bool isBall(communication::messages::types::EntityId id);
    bool isPlayer(communication::messages::types::EntityId id);
    auto idToSide(communication::messages::types::EntityId id) -> gameHandling::TeamSide;
    auto idToFantype(communication::messages::types::EntityId id) -> communication::messages::types::FanType;
}


#endif //SERVER_CONVERSIONS_H
