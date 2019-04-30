/**
 * @file MatchConfig.cpp
 * @author paul
 * @date 19.03.19
 * @brief Definition of the MatchConfig class
 */

#include "MatchConfig.hpp"

namespace communication::messages::broadcast {

    auto MatchConfig::getName() -> std::string {
        return "matchConfig";
    }

    MatchConfig::MatchConfig(int maxRounds, int playerTurnTimeout, int fanTurnTimeout, int playerPhaseTime,
                             int fanPhaseTime, int ballPhaseTime, float probGoal, float probThrowSuccess,
                             float probKnockOut, float probFoolAway, float probCatchSnitch, float probCatchQuaffle,
                             float probWrestQuaffle, float probExtraThinderblast, float probExtraCleansweep,
                             float probExtraComet, float probExtraNimbus, float probExtraFirebolt,
                             float probFoulFlacking, float probFoulHaversacking, float probFoulStooging,
                             float probFoulBlatching, float probFoulSnitchnip, float probFoulElf, float probFoulGoblin,
                             float probFoulTroll, float probFoulSnitch) : maxRounds{maxRounds},
                                                                          playerTurnTimeout{playerTurnTimeout},
                                                                          fanTurnTimeout{fanTurnTimeout},
                                                                          playerPhaseTime{playerPhaseTime},
                                                                          fanPhaseTime{fanPhaseTime},
                                                                          ballPhaseTime{ballPhaseTime},
                                                                          probGoal{probGoal},
                                                                          probThrowSuccess{probThrowSuccess},
                                                                          probKnockOut{probKnockOut},
                                                                          probFoolAway{probFoolAway},
                                                                          probCatchSnitch{probCatchSnitch},
                                                                          probCatchQuaffle{probCatchQuaffle},
                                                                          probWrestQuaffle{probWrestQuaffle},
                                                                          probExtraThinderblast{probExtraThinderblast},
                                                                          probExtraCleansweep{probExtraCleansweep},
                                                                          probExtraComet{probExtraComet},
                                                                          probExtraNimbus{probExtraNimbus},
                                                                          probExtraFirebolt{probExtraFirebolt},
                                                                          probFoulFlacking{probFoulFlacking},
                                                                          probFoulHaversacking{probFoulHaversacking},
                                                                          probFoulStooging{probFoulStooging},
                                                                          probFoulBlatching{probFoulBlatching},
                                                                          probFoulSnitchnip{probFoulSnitchnip},
                                                                          probFoulElf{probFoulElf},
                                                                          probFoulGoblin{probFoulGoblin},
                                                                          probFoulTroll{probFoulTroll},
                                                                          probFoulSnitch{probFoulSnitch} {}

    int MatchConfig::getMaxRounds() const {
        return maxRounds;
    }

    int MatchConfig::getPlayerTurnTimeout() const {
        return playerTurnTimeout;
    }

    int MatchConfig::getFanTurnTimeout() const {
        return fanTurnTimeout;
    }

    int MatchConfig::getPlayerPhaseTime() const {
        return playerPhaseTime;
    }

    int MatchConfig::getFanPhaseTime() const {
        return fanPhaseTime;
    }

    int MatchConfig::getBallPhaseTime() const {
        return ballPhaseTime;
    }

    float MatchConfig::getProbGoal() const {
        return probGoal;
    }

    float MatchConfig::getProbThrowSuccess() const {
        return probThrowSuccess;
    }

    float MatchConfig::getProbKnockOut() const {
        return probKnockOut;
    }

    float MatchConfig::getProbFoolAway() const {
        return probFoolAway;
    }

    float MatchConfig::getProbCatchSnitch() const {
        return probCatchSnitch;
    }

    float MatchConfig::getProbCatchQuaffle() const {
        return probCatchQuaffle;
    }

    float MatchConfig::getProbWrestQuaffle() const {
        return probWrestQuaffle;
    }

    float MatchConfig::getProbExtraThinderblast() const {
        return probExtraThinderblast;
    }

    float MatchConfig::getProbExtraCleansweep() const {
        return probExtraCleansweep;
    }

    float MatchConfig::getProbExtraComet() const {
        return probExtraComet;
    }

    float MatchConfig::getProbExtraNimbus() const {
        return probExtraNimbus;
    }

    float MatchConfig::getProbExtraFirebolt() const {
        return probExtraFirebolt;
    }

    float MatchConfig::getProbFoulFlacking() const {
        return probFoulFlacking;
    }

    float MatchConfig::getProbFoulHaversacking() const {
        return probFoulHaversacking;
    }

    float MatchConfig::getProbFoulStooging() const {
        return probFoulStooging;
    }

    float MatchConfig::getProbFoulBlatching() const {
        return probFoulBlatching;
    }

    float MatchConfig::getProbFoulSnitchnip() const {
        return probFoulSnitchnip;
    }

    float MatchConfig::getProbFoulElf() const {
        return probFoulElf;
    }

    float MatchConfig::getProbFoulGoblin() const {
        return probFoulGoblin;
    }

    float MatchConfig::getProbFoulTroll() const {
        return probFoulTroll;
    }

    float MatchConfig::getProbFoulSnitch() const {
        return probFoulSnitch;
    }

    bool MatchConfig::operator==(const MatchConfig &rhs) const {
        return maxRounds == rhs.maxRounds &&
               playerTurnTimeout == rhs.playerTurnTimeout &&
               fanTurnTimeout == rhs.fanTurnTimeout &&
               playerPhaseTime == rhs.playerPhaseTime &&
               fanPhaseTime == rhs.fanPhaseTime &&
               ballPhaseTime == rhs.ballPhaseTime &&
               probGoal == rhs.probGoal &&
               probThrowSuccess == rhs.probThrowSuccess &&
               probKnockOut == rhs.probKnockOut &&
               probFoolAway == rhs.probFoolAway &&
               probCatchSnitch == rhs.probCatchSnitch &&
               probCatchQuaffle == rhs.probCatchQuaffle &&
               probWrestQuaffle == rhs.probWrestQuaffle &&
               probExtraThinderblast == rhs.probExtraThinderblast &&
               probExtraCleansweep == rhs.probExtraCleansweep &&
               probExtraComet == rhs.probExtraComet &&
               probExtraNimbus == rhs.probExtraNimbus &&
               probExtraFirebolt == rhs.probExtraFirebolt &&
               probFoulFlacking == rhs.probFoulFlacking &&
               probFoulHaversacking == rhs.probFoulHaversacking &&
               probFoulStooging == rhs.probFoulStooging &&
               probFoulBlatching == rhs.probFoulBlatching &&
               probFoulSnitchnip == rhs.probFoulSnitchnip &&
               probFoulElf == rhs.probFoulElf &&
               probFoulGoblin == rhs.probFoulGoblin &&
               probFoulTroll == rhs.probFoulTroll &&
               probFoulSnitch == rhs.probFoulSnitch;
    }

    bool MatchConfig::operator!=(const MatchConfig &rhs) const {
        return !(rhs == *this);
    }

    void to_json(nlohmann::json &j, const MatchConfig &matchConfig) {
        j["maxRounds"] = matchConfig.getMaxRounds();
        j["timeouts"]["playerTurnTimeout"] = matchConfig.getPlayerTurnTimeout();
        j["timeouts"]["fanTurnTimeout"] = matchConfig.getFanTurnTimeout();
        j["timeouts"]["playerPhaseTime"] = matchConfig.getPlayerPhaseTime();
        j["timeouts"]["fanPhaseTime"] = matchConfig.getFanPhaseTime();
        j["timeouts"]["ballPhaseTime"] = matchConfig.getBallPhaseTime();
        j["probabilities"]["goal"] = matchConfig.getProbGoal();
        j["probabilities"]["throwSuccess"] = matchConfig.getProbThrowSuccess();
        j["probabilities"]["knockOut"] = matchConfig.getProbKnockOut();
        j["probabilities"]["foolAway"] = matchConfig.getProbFoolAway();
        j["probabilities"]["catchSniff"] = matchConfig.getProbCatchSnitch();
        j["probabilities"]["catchQuaffle"] = matchConfig.getProbCatchQuaffle();
        j["probabilities"]["wrestQuaffle"] = matchConfig.getProbWrestQuaffle();
        j["probabilities"]["extraMove"]["thinderblast"] = matchConfig.getProbExtraThinderblast();
        j["probabilities"]["extraMove"]["cleansweep11"] = matchConfig.getProbExtraCleansweep();
        j["probabilities"]["extraMove"]["comet260"] = matchConfig.getProbExtraComet();
        j["probabilities"]["extraMove"]["nimbus2001"] = matchConfig.getProbExtraNimbus();
        j["probabilities"]["extraMove"]["firebolt"] = matchConfig.getProbExtraFirebolt();
        j["probabilities"]["foulDetection"]["flacking"] = matchConfig.getProbFoulFlacking();
        j["probabilities"]["foulDetection"]["haversacking"] = matchConfig.getProbFoulHaversacking();
        j["probabilities"]["foulDetection"]["stooging"] = matchConfig.getProbFoulStooging();
        j["probabilities"]["foulDetection"]["blatching"] = matchConfig.getProbFoulBlatching();
        j["probabilities"]["foulDetection"]["snitchnip"] = matchConfig.getProbFoulSnitchnip();
        j["probabilities"]["fanFoulDetection"]["elfTeleportation"] = matchConfig.getProbFoulElf();
        j["probabilities"]["fanFoulDetection"]["goblinShock"] = matchConfig.getProbFoulGoblin();
        j["probabilities"]["fanFoulDetection"]["trollRoar"] = matchConfig.getProbFoulTroll();
        j["probabilities"]["fanFoulDetection"]["snitchSnatch"] = matchConfig.getProbFoulSnitch();
    }

    void from_json(const nlohmann::json &j, MatchConfig &matchConfig) {
        matchConfig = MatchConfig{
            j.at("maxRounds").get<int>(),
            j.at("timeouts").at("playerTurnTimeout").get<int>(),
            j.at("timeouts").at("fanTurnTimeout").get<int>(),
            j.at("timeouts").at("playerPhaseTime").get<int>(),
            j.at("timeouts").at("fanPhaseTime").get<int>(),
            j.at("timeouts").at("ballPhaseTime").get<int>(),
            j.at("probabilities").at("goal").get<float>(),
            j.at("probabilities").at("throwSuccess").get<float>(),
            j.at("probabilities").at("knockOut").get<float>(),
            j.at("probabilities").at("foolAway").get<float>(),
            j.at("probabilities").at("catchSniff").get<float>(),
            j.at("probabilities").at("catchQuaffle").get<float>(),
            j.at("probabilities").at("wrestQuaffle").get<float>(),
            j.at("probabilities").at("extraMove").at("thinderblast").get<float>(),
            j.at("probabilities").at("extraMove").at("cleansweep11").get<float>(),
            j.at("probabilities").at("extraMove").at("comet260").get<float>(),
            j.at("probabilities").at("extraMove").at("nimbus2001").get<float>(),
            j.at("probabilities").at("extraMove").at("firebolt").get<float>(),
            j.at("probabilities").at("foulDetection").at("flacking").get<float>(),
            j.at("probabilities").at("foulDetection").at("haversacking").get<float>(),
            j.at("probabilities").at("foulDetection").at("stooging").get<float>(),
            j.at("probabilities").at("foulDetection").at("blatching").get<float>(),
            j.at("probabilities").at("foulDetection").at("snitchnip").get<float>(),
            j.at("probabilities").at("fanFoulDetection").at("elfTeleportation").get<float>(),
            j.at("probabilities").at("fanFoulDetection").at("goblinShock").get<float>(),
            j.at("probabilities").at("fanFoulDetection").at("trollRoar").get<float>(),
            j.at("probabilities").at("fanFoulDetection").at("snitchSnatch").get<float>()
        };
    }
}
