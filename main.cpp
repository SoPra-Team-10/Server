#include <iostream>

#include <Communication/Communicator.hpp>

int main() {
    communication::Communicator communicator{8080};


    std::cout << "Started server!" << std::endl;

    while (true);
}
