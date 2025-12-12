#include "ServerAppInterface.hpp"

#include <iostream>
#include <string>

ServerAppInterface::ServerAppInterface(std::shared_ptr<ServerSideEngine> engine) : engine(engine) {
    // TO-DO implement constructor
}

void ServerAppInterface::readCommands() {
    // TO-DO implement the read commands method
    std::string command;
    
    while (true) {
        std::cout << "> ";
        
        // read from command line
        std::cin >> command;

        // if the command is quit, terminate the program       
        if (command == "quit") {
            engine->shutdown();
            break;
        }

        std::cout << "unrecognized command!" << std::endl;
    }
}