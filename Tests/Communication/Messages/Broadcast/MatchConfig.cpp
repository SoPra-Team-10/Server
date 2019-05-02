#include <gtest/gtest.h>
#include <Communication/Messages/Broadcast/MatchConfig.hpp>

using namespace communication::messages;

using T = broadcast::MatchConfig;

TEST(CommunicationMessagesBroadcastMatchConfig, SerializeDeserialize) {
    T orig{1,2,3,4,5,6,7.1,8.1,9.1,10.1,11.1,12.1,13.1,14.1,15.1,16.1,
           17.1,18.1,19.1,20.1,21.1,22.1,23.1,24.1,25.1,26.1};
    std::string ser;
    nlohmann::json json;
    T comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T>());
    EXPECT_EQ(comp, orig);
}
