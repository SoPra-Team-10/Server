/**
 * @file Logging.hpp
 * @author paul
 * @date 01.05.19
 * @brief Logging @TODO
 */

#ifndef SERVER_LOGGING_HPP
#define SERVER_LOGGING_HPP

#include <ostream>

class Logging {
public:
    Logging(std::ostream &ostream, unsigned int loggingLevel);

    void error();
    void warn();
    void log();
    void debug();
private:
    void logImpl(const std::string &string);

    std::ostream &ostream;
    unsigned int loggingLevel;
};


#endif //SERVER_LOGGING_HPP
