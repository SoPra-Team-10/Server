/**
 * @file ArgumentParser.hpp
 * @author paul
 * @date 01.05.19
 * @brief ArgumentParser @TODO
 */

#ifndef SERVER_ARGUMENTPARSER_HPP
#define SERVER_ARGUMENTPARSER_HPP

#include <string>
#include <optional>

namespace util {
    class ArgumentParser {
    public:
        ArgumentParser(int argc, char *argv[]);
        auto getMatch() const -> std::string;
        auto getPort() const -> uint16_t;
        auto getVerbosity() const -> unsigned int;

        static void printHelp();
    private:
        std::string match;
        uint16_t port;
        unsigned int verbosity;
    };
}

#endif //SERVER_ARGUMENTPARSER_HPP
