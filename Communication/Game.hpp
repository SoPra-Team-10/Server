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
    private:
        Communicator &communicator;

        template <typename T>
        void onPayload(const T &, int id);

        void sendLeft(const messages::Message &message);
        void sendRight(const messages::Message &message);
        void sendAll(const messages::Message &message);
        void sendSingle(const messages::Message &message, int id);

        std::map<int, Client> clients;
        std::pair<std::optional<int>, std::optional<int>> players;
    };
}
#endif //SERVER_GAME_HPP
