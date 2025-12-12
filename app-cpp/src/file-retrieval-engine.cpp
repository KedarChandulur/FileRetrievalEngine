#include <iostream>

#include "IndexStore.hpp"
#include "ProcessingEngine.hpp"
#include "AppInterface.hpp"

int main(int argc, char** argv)
{
    int num_workers;

    if (argc != 2) 
    {
        std::cerr << "USE: ./file-retrieval-engine <number of worker threads>" << std::endl;
        return 1;
    }

    num_workers = std::atoi(argv[1]);

    std::shared_ptr<IndexStore> store = std::make_shared<IndexStore>();
    std::shared_ptr<ProcessingEngine> engine = std::make_shared<ProcessingEngine>(store, num_workers);
    std::shared_ptr<AppInterface> interface = std::make_shared<AppInterface>(engine);

    interface->readCommands();

    return 0;
}