#include "ClientAppInterface.hpp"

#include <iostream>
#include <string>

ClientAppInterface::ClientAppInterface(std::shared_ptr<ClientSideEngine> engine) : engine(engine) {
    // TO-DO implement constructor
}

void ClientAppInterface::readCommands() {
    // TO-DO implement the read commands method
    std::string command;
    
    while (true) {
        std::cout << "> ";
        
        // read from command line
        std::cin >> command;

        // if the command is quit, terminate the program       
        if (command == "quit") {
            break;
        }
        
        // if the command begins with index, index the files from the specified directory
        if (command.size() >= 5 && command.substr(0, 5) == "index") {
            // TO-DO call the client side engine method to index the documents from a folder
            continue;
        }

        // if the command begins with search, search for files that matches the query
        if (command.size() >= 6 && command.substr(0, 6) == "search") {
            // TO-DO call the client side engine method to search for documents given a complex query
            continue;
        }

        std::cout << "unrecognized command!" << std::endl;
    }
}