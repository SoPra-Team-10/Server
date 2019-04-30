/**
 * @file Game.hpp
 * @author paul
 * @date 28.04.19
 * @brief Game @TODO
 */

#ifndef SERVER_GAME_HPP
#define SERVER_GAME_HPP

#include <utility>
#include <Communication/Messages/Message.h>

namespace communication {
    class Communicator;

    struct Client {
        std::string userName, password;
        bool isAi;
        std::vector<std::string> mods;
    };

    class Game {
    public:
        Game(Communicator &communicator, Client client, int id);
        void addSpectator(Client client, int id);

        void onMessage(const messages::Message &message, int id);

        void kickUser(int id);
    private:
        void sendLeft(const messages::Payload &payload);
        void sendRight(const messages::Payload  &payload);
        void sendAll(const messages::Payload  &payload);
        void sendSingle(const messages::Payload &payload, int id);

        template <typename T>
        void onPayload(const T &, int id);

        Communicator &communicator;

        std::map<int, Client> clients;
        std::pair<std::optional<int>, std::optional<int>> players;
    };
}
#endif //SERVER_GAME_HPP