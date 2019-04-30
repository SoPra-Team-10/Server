#include <gtest/gtest.h>
#include <Communication/Messages/Broadcast/MatchFinish.hpp>

using namespace communication::messages;

using T = broadcast::MatchFinish;

TEST(CommunicationMessagesBroadcastMatchFinish, SerializeDeserialize) {
    T orig{1,2,3,"Test",types::VictoryReason::MOST_POINTS};
    std::string ser;
    nlohmann::json json;
    T comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T>());
    EXPECT_EQ(comp, orig);
}
