#include <getopt.h>
#include <iostream>

#include <Communication/Communicator.hpp>
#include <Util/ArgumentParser.hpp>

int main(int argc, char *argv[]) {
    //std::string match;
    uint16_t port;
    //unsigned int verbosity;
    try {
        util::ArgumentParser argumentParser{argc, argv};
        //match = argumentParser.getMatch();
        port = argumentParser.getPort();
        //verbosity = argumentParser.getVerbosity();
    } catch (std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }


    communication::Communicator communicator{port};


    std::cout << "Started server!" << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::hours{100});
    }
}
