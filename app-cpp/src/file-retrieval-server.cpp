#include <iostream>
#include <memory>

#include "IndexStore.hpp"
#include "ServerSideEngine.hpp"
#include "ServerAppInterface.hpp"

int main(int argc, char** argv)
{
    std::shared_ptr<IndexStore> store = std::make_shared<IndexStore>();
    std::shared_ptr<ServerSideEngine> engine = std::make_shared<ServerSideEngine>(store);
    std::shared_ptr<ServerAppInterface> interface = std::make_shared<ServerAppInterface>(engine);

    // create a thread that creates and server TCP/IP socket and listenes to connections
    engine->initialize();

    // read commands from the user
    interface->readCommands();

    return 0;
}