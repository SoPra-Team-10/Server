/**
 * @file Game.hpp
 * @author paul
 * @date 28.04.19
 * @brief Declaration of the Game class
 */

#ifndef SERVER_LOBBY_HPP
#define SERVER_LOBBY_HPP

#include <utility>
#include <set>
#include <SopraMessages/Message.hpp>
#include <SopraMessages/Replay.hpp>
#include <SopraMessages/ReplayWithSnapshot.h>
#include <Util/Logging.hpp>
#include <Game/Game.h>

namespace communication {
    class Communicator;

    struct Client {
        std::string userName, password;
        bool isAi;
        std::set<messages::types::Mods> mods;
    };

    enum class LobbyState {
        INITIAL, WAITING_FORMATION, GAME, PAUSE, FINISHED
    };

    class Lobby {
    public:
        Lobby(const std::string &name, const std::string &startTime,
                Communicator &communicator, const Client& client, int id, util::Logging &log,
                const messages::broadcast::MatchConfig &matchConfig);
        void addSpectator(Client client, int id);

        void onMessage(const messages::Message &message, int id);
        void onLeave(int id);

        auto getUserInLobby() const -> int;
        auto isMatchStarted() const -> bool;
    private:
        void kickUser(int id);
        void sendAll(const messages::Payload &payload);
        void sendSingle(const messages::Payload &payload, int id);
        void sendSingle(const messages::broadcast::Replay &payload, int id);
        void sendSingle(const messages::mods::unicast::ReplayWithSnapshot &payload, int id);
        void sendError(const std::string &payloadReason, const std::string &msg, int id);
        void sendWarn(const std::string &payloadReason, const std::string &msg, int id);

        template <typename T>
        void onPayload(const T &, int id);

        void onTimeout(TeamSide teamSide);
        void onWin(TeamSide teamSide, communication::messages::types::VictoryReason victoryReason);

        Communicator &communicator;
        LobbyState state;
        std::string name;

        std::pair<messages::broadcast::Replay, messages::mods::unicast::ReplayWithSnapshot> replay;
        std::map<int, Client> clients;
        std::pair<std::optional<int>, std::optional<int>> players;
        std::pair<std::optional<communication::messages::request::TeamConfig>,
            std::optional<communication::messages::request::TeamConfig>> teamConfigs;
        std::pair<std::optional<communication::messages::request::TeamFormation>,
        std::optional<communication::messages::request::TeamFormation>> teamFormations;
        std::optional<Game> game;
        std::optional<communication::messages::broadcast::Next> lastNext;
        const messages::broadcast::MatchConfig matchConfig;
        util::Logging &log;
    };
}
#endif //SERVER_LOBBY_HPP
