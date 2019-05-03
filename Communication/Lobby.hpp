/**
 * @file Game.hpp
 * @author paul
 * @date 28.04.19
 * @brief Declaration of the Game class
 */

#ifndef SERVER_LOBBY_HPP
#define SERVER_LOBBY_HPP

#include <utility>
#include <SopraMessages/Message.hpp>
#include <Util/Logging.hpp>
#include <Game/Game.h>

namespace communication {
    class Communicator;

    struct Client {
        std::string userName, password;
        bool isAi;
        std::vector<std::string> mods;
    };

    enum class LobbyState {
        INITIAL, WAITING_FORMATION, GAME, PAUSE, FINISHED
    };

    class Lobby {
    public:
        Lobby(Communicator &communicator, Client client, int id, util::Logging &log,
                const messages::broadcast::MatchConfig &matchConfig);
        void addSpectator(Client client, int id);

        void onMessage(const messages::Message &message, int id);
        void onLeave(int id);

        void kickUser(int id);
    private:
        void sendAll(const messages::Payload  &payload);
        void sendSingle(const messages::Payload &payload, int id);

        template <typename T>
        void onPayload(const T &, int id);

        void onTimeout(TeamSide teamSide);
        void onWin(TeamSide teamSide, communication::messages::types::VictoryReason victoryReason);

        Communicator &communicator;
        LobbyState state;

        std::map<int, Client> clients;
        std::pair<std::optional<int>, std::optional<int>> players;
        std::pair<std::optional<communication::messages::request::TeamConfig>,
            std::optional<communication::messages::request::TeamConfig>> teamConfigs;
        std::pair<std::optional<communication::messages::request::TeamFormation>,
        std::optional<communication::messages::request::TeamFormation>> teamFormations;
        std::optional<Game> game;
        std::optional<communication::messages::broadcast::Next> lastNext;
        const messages::broadcast::MatchConfig &matchConfig;
        util::Logging &log;
    };
}
#endif //SERVER_LOBBY_HPP
