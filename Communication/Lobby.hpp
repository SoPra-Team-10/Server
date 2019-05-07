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
#include <SopraMessages/ReplayMessage.h>

namespace communication {
    class Communicator;

    /**
     * A client represents one client and all the relevant information
     */
    struct Client {
        std::string userName, password;
        bool isAi;
        std::set<messages::types::Mods> mods;
    };

    /**
     * The current state of the lobby state machine
     */
    enum class LobbyState {
        INITIAL, WAITING_FORMATION, GAME, PAUSE, FINISHED
    };

    /**
     * Represents a single lobby and all relevant information
     */
    class Lobby {
    public:
        /**
         * CTor: Initialize the Lobby, and add the first client.
         * @param name the name of the lobby
         * @param startTime the timestamp of the message that started the lobby
         * @param communicator a reference to the communicator to send messages
         * @param client the first client in the lobby
         * @param id the id of the client as send by the MessageHandler
         * @param log the log in which to write
         * @param matchConfig the matchConfig for the lobby
         */
        Lobby(const std::string &name, const std::string &startTime,
                Communicator &communicator, const Client& client, int id, util::Logging &log,
                const messages::broadcast::MatchConfig &matchConfig);

        /**
         * Add another spectator to the lobby, this player is always a spectator at first.
         * @param client the client which should get added
         * @param id the id of the player as send by the MessageHandler
         */
        void addSpectator(Client client, int id);

        /**
         * Function that gets called on a new message (except JoinRequest which are handled in the Communicator)
         */
        void onMessage(const messages::Message &message, int id);

        /**
         * An event that gets called when a client leaves
         * @param id the id of the player
         * @return true if the lobby is empty after the player left and thus if the lobby should be closed
         */
        bool onLeave(int id);

        /**
         * Get the number of users in the lobby
         * @return the number of users in the lobby
         */
        auto getUserInLobby() const -> int;

        /**
         * Return if currently a game is active
         * @return wheter the game is actively running
         */
        auto isMatchStarted() const -> bool;

        /**
         * Get the name of the lobby
         * @return the name of the lobby
         */
        auto getName() const -> std::string;
    private:
        void kickUser(int id);
        void sendAll(const messages::Payload &payload);
        void sendSingle(const messages::Payload &payload, int id);
        void sendSingle(const messages::ReplayPayload &payload, int id);
        void sendError(const std::string &payloadReason, const std::string &msg, int id);
        void sendWarn(const std::string &payloadReason, const std::string &msg, int id);

        template <typename T>
        void onPayload(const T &, int id);

        void onTeamFormationTimeout();
        void onTimeout(gameHandling::TeamSide teamSide);
        void onWin(gameHandling::TeamSide teamSide, communication::messages::types::VictoryReason victoryReason);

        util::Logging &log;

        LobbyState state;
        Communicator &communicator;
        const messages::broadcast::MatchConfig matchConfig;
        std::map<int, Client> clients;
        std::string name;

        std::pair<std::optional<communication::messages::request::TeamConfig>,
                std::optional<communication::messages::request::TeamConfig>> teamConfigs;
        std::pair<std::optional<int>, std::optional<int>> players;
        util::Timer teamFormationTimer;
        std::pair<std::optional<communication::messages::request::TeamFormation>,
                std::optional<communication::messages::request::TeamFormation>> teamFormations;

        std::optional<gameHandling::Game> game;

        std::pair<messages::broadcast::Replay, messages::mods::unicast::ReplayWithSnapshot> replay;
        std::optional<communication::messages::broadcast::Next> lastNext;
    };
}
#endif //SERVER_LOBBY_HPP
