#ifndef SERVER_APP_INTERFACE_H
#define SERVER_APP_INTERFACE_H

#include <memory>

#include "ServerSideEngine.hpp"

class ServerAppInterface
{
    std::shared_ptr<ServerSideEngine> engine;

    public:
        // constructor
        ServerAppInterface(std::shared_ptr<ServerSideEngine> engine);

        // default virtual destructor
        virtual ~ServerAppInterface() = default;

        // read commands from the user
        void readCommands();
};

#endif