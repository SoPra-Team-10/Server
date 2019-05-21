/**
 * @file CommonMessages.cpp
 * @author paul
 * @date 13.05.19
 * @brief CommonMessages @TODO
 */

#include "CommonMessages.hpp"

auto getMatchConfig() -> communication::messages::broadcast::MatchConfig {
    return communication::messages::broadcast::MatchConfig
        {1000,1000,1000,1000,1000,1000,1000,1000,1000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
}

auto getTeamConfig(bool left) -> communication::messages::request::TeamConfig {
    using namespace communication::messages::types;

    return communication::messages::request::TeamConfig{
        left ? "abc" : "def",
        left ? "ghi" : "jkl",
        left ? "000000" : "FFFFFF",
        left ? "100000" : "1FFFFF",
        "",
        1,
        1,
        1,
        4,
        {"abc", Broom::CLEANSWEEP11, Sex::F},
        {"abc", Broom::COMET260, Sex::F},
        {"abc", Broom::FIREBOLT, Sex::F},
        {"abc", Broom::NIMBUS2001, Sex::M},
        {"abc", Broom::TINDERBLAST, Sex::M},
        {"abc", Broom::CLEANSWEEP11, Sex::M},
        {"abc", Broom::CLEANSWEEP11, Sex::M},
    };

}

auto getTeamFormation(bool left) -> communication::messages::request::TeamFormation {
    return communication::messages::request::TeamFormation{
        left ? 3 : 13,
        0,
        left ? 4 : 12,
        0,
        left ? 5 : 11,
        0,
        left ? 6 : 10,
        0,
        left ? 7 : 9,
        0,
        left ? 3 : 13,
        1,
        left ? 4 : 12,
        1
    };
}
