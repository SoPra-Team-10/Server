/**
 * @file DeltaRequest.cpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaRequest @TODO
 */

#include "DeltaRequest.hpp"

namespace communication::messages::request {

    auto DeltaRequest::getName() -> std::string {
        return "deltaRequest";
    }

    DeltaRequest::DeltaRequest(types::DeltaType deltaType, bool success, int xPosOld, int yPosOld, int xPosNew,
                               int yPosNew, types::EntityId activeEntity, types::EntityId passiveEntity) : deltaType(
            deltaType), success(success), xPosOld(xPosOld), yPosOld(yPosOld), xPosNew(xPosNew), yPosNew(yPosNew),
                                                                                                           activeEntity(
                                                                                                                   activeEntity),
                                                                                                           passiveEntity(
                                                                                                                   passiveEntity) {}

    types::DeltaType DeltaRequest::getDeltaType() const {
        return deltaType;
    }

    bool DeltaRequest::isSuccess() const {
        return success;
    }

    int DeltaRequest::getXPosOld() const {
        return xPosOld;
    }

    int DeltaRequest::getYPosOld() const {
        return yPosOld;
    }

    int DeltaRequest::getXPosNew() const {
        return xPosNew;
    }

    int DeltaRequest::getYPosNew() const {
        return yPosNew;
    }

    types::EntityId DeltaRequest::getActiveEntity() const {
        return activeEntity;
    }

    types::EntityId DeltaRequest::getPassiveEntity() const {
        return passiveEntity;
    }

    void to_json(nlohmann::json &j, const DeltaRequest &deltaRequest) {
        j["deltaType"] = types::toString(deltaRequest.getDeltaType());
        j["success"] = deltaRequest.isSuccess();
        j["xPosOld"] = deltaRequest.getXPosOld();
        j["yPosOld"] = deltaRequest.getYPosOld();
        j["xPosNew"] = deltaRequest.getXPosNew();
        j["yPosNew"] = deltaRequest.getYPosNew();
        j["activeEntity"] = types::toString(deltaRequest.getActiveEntity()),
        j["passiveEntity"] = types::toString(deltaRequest.getPassiveEntity());
    }

    void from_json(const nlohmann::json &j, DeltaRequest &deltaRequest) {
        deltaRequest = DeltaRequest{
            types::fromStringDeltaType(j.at("deltaType").get<std::string>()),
            j.at("success").get<bool>(),
            j.at("xPosOld").get<int>(),
            j.at("yPosOld").get<int>(),
            j.at("xPosNew").get<int>(),
            j.at("yPosNew").get<int>(),
            types::fromStringEntityId(j.at("activeEntity").get<std::string>()),
            types::fromStringEntityId(j.at("passiveEntity").get<std::string>())
        };
    }
}
