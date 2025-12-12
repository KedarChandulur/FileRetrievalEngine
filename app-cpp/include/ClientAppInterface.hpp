#ifndef CLIENT_APP_INTERFACE_H
#define CLIENT_APP_INTERFACE_H

#include <memory>

#include "ClientSideEngine.hpp"

class ClientAppInterface
{
    std::shared_ptr<ClientSideEngine> engine;

    public:
        // constructor
        ClientAppInterface(std::shared_ptr<ClientSideEngine> engine);

        // default virtual destructor
        virtual ~ClientAppInterface() = default;

        // read commands from the user
        void readCommands();
};

#endif