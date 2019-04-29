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
        Game(std::string lobbyName, Client client);
        void addSecond(Client client);

        void onMessageLeft(const messages::Message &message);
        void onMessageRight(const messages::Message &message);

        auto getLobbyName() const -> std::string;

        auto getLeftId() const -> int;
        auto getRightId() const -> int;
    private:
        std::string lobbyName;
        std::pair<Client, std::optional<Client>> clients;
    };
}
#endif //SERVER_GAME_HPP
