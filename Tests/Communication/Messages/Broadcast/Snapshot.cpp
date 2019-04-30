#include <gtest/gtest.h>
#include <Communication/Messages/Broadcast/Snapshot.hpp>

using namespace communication::messages;

using T = broadcast::Snapshot;

TEST(CommunicationMessagesBroadcastSnapshot, SerializeDeserialize) {
    T orig{types::PhaseType::FAN_PHASE, {}, 17, {}, {},
           1,2,3,4,5,6,7,8};
    std::string ser;
    nlohmann::json json;
    T comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T>());
    EXPECT_EQ(comp, orig);
}
