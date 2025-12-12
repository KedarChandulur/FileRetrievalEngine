#include <iostream>

#include "ClientSideEngine.hpp"
#include "ClientAppInterface.hpp"

int main(int argc, char** argv)
{
    std::shared_ptr<ClientSideEngine> engine = std::make_shared<ClientSideEngine>();
    std::shared_ptr<ClientAppInterface> interface = std::make_shared<ClientAppInterface>(engine);

    interface->readCommands();

    return 0;
}