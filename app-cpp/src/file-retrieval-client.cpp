#include <iostream>

#include "ClientSideEngine.hpp"
#include "Utilities.hpp"
#include "ClientAppInterface.hpp"

int main(int argc, char** argv)
{
    std::shared_ptr<ClientSideEngine> engine = std::make_shared<ClientSideEngine>();
    std::shared_ptr<ClientAppInterface> interface = std::make_shared<ClientAppInterface>(engine);

    int _minFolder = 0;
    int _maxFolder = 0;
    int numWorkers = 1;

    if (argc > 2)
    {
        _minFolder = atoi(argv[1]);
        _maxFolder = atoi(argv[2]);
    }

    if (_minFolder == 0 || _maxFolder == 0 || _minFolder > _maxFolder)
    {
        std::cout << "Enter valid range for the index folders\n";
        return 0;
    }

    engine->InitializeLocalThreads(_minFolder, _maxFolder, numWorkers);

    //std::cout << _minFolder << " " << _maxFolder << std::endl;

    if (argc > 4)
    {
        std::string _address(argv[3]);
        std::string _port(argv[4]);

        std::cout << "Connect directly called with address: " << _address << " and port: " << _port << "\n";
        engine->openConnection(_address, _port);
    }

    interface->readCommands();

    return 0;
}