#include <gtest/gtest.h>
#include <Communication/Messages/Request/TeamConfig.hpp>

using namespace communication::messages;

using T0 = request::TeamConfig;
using T1 = request::Player;

TEST(CommunicationMessagesRequestTeamConfig, TeamConfigSerializeDeserialize) {
    T0 orig{"a","b","c","d","e",1,2,3,4,{},{},{},{},{},{},{}};
    std::string ser;
    nlohmann::json json;
    T0 comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T0>());
    EXPECT_EQ(comp, orig);
}

TEST(CommunicationMessagesRequestTeamConfig, PlayerSerializeDeserialize) {
    T1 orig{"Test", types::Broom::FIREBOLT, types::Sex::F};
    std::string ser;
    nlohmann::json json;
    T1 comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T1>());
    EXPECT_EQ(comp, orig);
}
