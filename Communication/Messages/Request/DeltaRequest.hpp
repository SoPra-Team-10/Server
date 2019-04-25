/**
 * @file DeltaRequest.hpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaRequest @TODO
 */

#ifndef SERVER_DELTAREQUEST_HPP
#define SERVER_DELTAREQUEST_HPP

#include <string>
#include <Lib/json.hpp>
#include <Communication/Messages/types.hpp>

namespace communication::messages::request {
    class DeltaRequest {
    public:
        DeltaRequest(types::DeltaType deltaType, bool success, int xPosOld, int yPosOld, int xPosNew, int yPosNew,
                     types::EntityId activeEntity, types::EntityId passiveEntity);

        types::DeltaType getDeltaType() const;

        bool isSuccess() const;

        int getXPosOld() const;

        int getYPosOld() const;

        int getXPosNew() const;

        int getYPosNew() const;

        types::EntityId getActiveEntity() const;

        types::EntityId getPassiveEntity() const;

        static auto getName() -> std::string;
    private:
        types::DeltaType deltaType;
        bool success;
        int xPosOld, yPosOld, xPosNew, yPosNew;
        types::EntityId activeEntity, passiveEntity;
    };

    void to_json(nlohmann::json &j, const DeltaRequest &deltaRequest);
    void from_json(const nlohmann::json &j, DeltaRequest &deltaRequest);
}

#endif //SERVER_DELTAREQUEST_HPP
