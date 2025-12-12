#include <iostream>
#include <string>

#include "Utilities.hpp"
#include "ServerAppInterface.hpp"

ServerAppInterface::ServerAppInterface(std::shared_ptr<ServerSideEngine> engine)
    : engine(engine) 
{
}

void ServerAppInterface::readCommands() 
{
    // TO-DO implement the read commands method
    std::string command;
    
    while (true)
    {
        std::cout << "> ";
        
        // read from command line
        //getline(std::cin, command);

        if (!std::getline(std::cin, command))
        {
            // Handle EOF
            std::cout << "Input terminated. Quitting...\n";
            engine->shutdown();
            break;
        }

        // if the command is quit, terminate the program       
        if (command == "quit") 
        {
            engine->shutdown();
            break;
        }

        if (command.size() == 4 && command.substr(0, 4) == "test")
        {
            engine->OnServerQuitEvent();
            continue;
        }

        // if the command begins with list, list all the connected clients
        if (command.size() >= 4 && command.substr(0, 4) == "list")
        {
            // TO-DO call the list method from the server to retrieve the clients information
            // print the clients information
            engine->listClients();
            continue;
        }

        std::cout << "unrecognized command!" << std::endl;
    }
}