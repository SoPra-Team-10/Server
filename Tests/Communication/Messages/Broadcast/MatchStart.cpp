#include <gtest/gtest.h>
#include <Communication/Messages/Broadcast/MatchStart.hpp>

using namespace communication::messages;

using T = broadcast::MatchStart;

TEST(CommunicationMessagesBroadcastMatchStart, SerializeDeserialize) {
    T orig{broadcast::MatchConfig{}, request::TeamConfig{}, request::TeamConfig{},"a","b"};
    std::string ser;
    nlohmann::json json;
    T comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T>());
    EXPECT_EQ(comp, orig);
}
