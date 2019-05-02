/**
 * @file Logging.hpp
 * @author paul
 * @date 01.05.19
 * @brief Logging @TODO
 */

#ifndef SERVER_LOGGING_HPP
#define SERVER_LOGGING_HPP

#include <ostream>

namespace util {
    class Logging {
    public:
        Logging(std::ostream &ostream, unsigned int loggingLevel);

        void error(const std::string &msg);

        void warn(const std::string &msg);

        void info(const std::string &msg);

        void debug(const std::string &msg);

    private:
        void logImpl(const std::string &string, unsigned int level, const std::string &colorFormat);

        std::ostream &ostream;
        unsigned int loggingLevel;
    };
}

#endif //SERVER_LOGGING_HPP
