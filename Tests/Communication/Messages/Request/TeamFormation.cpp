#include <gtest/gtest.h>
#include <Communication/Messages/Request/TeamFormation.hpp>

using namespace communication::messages;

using T = request::TeamFormation;

TEST(CommunicationMessagesRequestTeamFormation, SerializeDeserialize) {
    T orig{1,2,3,4,5,6,7,8,9,10,11,12,13,14};
    std::string ser;
    nlohmann::json json;
    T comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T>());
    EXPECT_EQ(comp, orig);
}
