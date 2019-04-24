/**
 * @file Next.hpp
 * @author paul
 * @date 19.03.19
 * @brief Next @TODO
 */

#ifndef SERVER_NEXT_HPP
#define SERVER_NEXT_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class Next {
    public:

        static auto getName() -> std::string;
    private:

    };
}

#endif //SERVER_NEXT_HPP
