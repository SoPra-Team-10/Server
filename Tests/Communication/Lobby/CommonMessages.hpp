/**
 * @file CommonMessages.hpp
 * @author paul
 * @date 13.05.19
 * @brief CommonMessages @TODO
 */

#ifndef SERVER_COMMONMESSAGES_HPP
#define SERVER_COMMONMESSAGES_HPP

#include <SopraMessages/MatchConfig.hpp>
#include <SopraMessages/TeamConfig.hpp>
#include <SopraMessages/TeamFormation.hpp>

auto getMatchConfig() -> communication::messages::broadcast::MatchConfig;
auto getTeamConfig(bool left) -> communication::messages::request::TeamConfig;
auto getTeamFormation(bool left) -> communication::messages::request::TeamFormation;

#endif //SERVER_COMMONMESSAGES_HPP
