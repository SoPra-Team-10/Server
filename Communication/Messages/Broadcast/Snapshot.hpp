/**
 * @file Snapshot.hpp
 * @author paul
 * @date 19.03.19
 * @brief Snapshot @TODO
 */

#ifndef SERVER_SNAPSHOT_HPP
#define SERVER_SNAPSHOT_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class Snapshot {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_SNAPSHOT_HPP
