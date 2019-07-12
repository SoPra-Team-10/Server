/**
 * @file Lobby.hpp
 * @author paul
 * @date 28.04.19
 * @brief Declaration of the Lobby class
 */

#ifndef SERVER_LOBBY_HPP
#define SERVER_LOBBY_HPP

#include <utility>
#include <set>
#include <chrono>
#include <queue>
#include <SopraMessages/Message.hpp>
#include <SopraMessages/Replay.hpp>
#include <SopraMessages/ReplayWithSnapshot.h>
#include <SopraUtil/Logging.hpp>
#include <Game/Game.h>
#include <SopraMessages/ReplayMessage.h>
#include "AnimationQueue.h"

namespace communication {
    class Communicator;

    /**
     * A client represents one client and all the relevant information
     */
    struct Client {
        std::string userName; ///< Name of the user, as given at the login
        std::string password; ///< Password of the user, absolutely useless
        bool isAi; ///< True if the player is an AI (AI players can't pause)
        std::set<messages::types::Mods> mods; ///< Collection of all mods that the client supports
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
         * Checks if a user with the same name and password is already in the lobby
         * @param client the new client
         * @param id the id of the new client
         * @return the old id if this is a rejoin
         */
        auto reAddUser(const Client& client, int id) -> std::optional<int>;

        /**
         * Function that gets called on a new message (except JoinRequest which are handled in the Communicator)
         */
        void onMessage(const messages::Message &message, int id);

        /**
         * An event that gets called when a client leaves
         * @param id the id of the player
         * @return true if the lobby is empty after the player left and thus if the lobby should be closed
         */
        void onLeave(int id);

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
        void onTimeout(communication::messages::types::EntityId entityId,
                communication::messages::types::PhaseType phaseType);
        void onWin(gameModel::TeamSide teamSide, communication::messages::types::VictoryReason victoryReason);
        void onFatalError(const std::string& error);
        void modifySnapshotsAddToLogAndSend(std::queue<communication::messages::broadcast::Snapshot> snapshots);
        void onLeaveAfterTimeout(int id, std::optional<std::shared_ptr<util::Timer>> timer = std::nullopt);

        auto getSpectators() const -> std::vector<std::string>;

        util::Logging &log;
        AnimationQueue animationQueue;

        LobbyState state;
        Communicator &communicator;
        const messages::broadcast::MatchConfig matchConfig;
        std::map<int, Client> clients;
        std::string name;
        std::list<std::pair<std::string, std::string>> lastTenMessages;
        std::map<int, std::shared_ptr<util::Timer>> leaveTimers;

        std::pair<std::optional<communication::messages::request::TeamConfig>,
                std::optional<communication::messages::request::TeamConfig>> teamConfigs;
        std::pair<std::optional<int>, std::optional<int>> players;
        util::Timer teamFormationTimer;
        std::pair<std::optional<communication::messages::request::TeamFormation>,
                std::optional<communication::messages::request::TeamFormation>> teamFormations;

        std::optional<gameHandling::Game> game;

        std::pair<messages::broadcast::Replay, messages::mods::unicast::ReplayWithSnapshot> replay;
        std::optional<communication::messages::broadcast::Next> lastNext;
        std::optional<communication::messages::broadcast::Snapshot> lastSnapshot;
    };
}
#endif //SERVER_LOBBY_HPP
