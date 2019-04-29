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
    struct Client {
        std::string userName, password;
        bool isAi;
        std::vector<std::string> mods;
        int clientId;
    };

    class Game {
    public:
        explicit Game(Client client);
        void addSpectator(Client client);

        void onMessage(const messages::Message &message, int id);

    private:
        template <typename T>
        void onPayload(const T &, int id);

        std::vector<Client> spectators;
        std::pair<Client, std::optional<Client>> players;
    };
}
#endif //SERVER_GAME_HPP
