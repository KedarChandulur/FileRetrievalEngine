#ifndef SERVER_SIDE_ENGINE_H
#define SERVER_SIDE_ENGINE_H

#include <memory>

#include "IndexStore.hpp"

class ServerSideEngine {
    // TO-DO keep track of the dispatcher thread, worker threads and the connected clients
    // TO-DO keep track of the index store
    std::shared_ptr<IndexStore> store;

    public:
        // constructor
        ServerSideEngine(std::shared_ptr<IndexStore> store);

        // default virtual destructor
        virtual ~ServerSideEngine() = default;

        // TO-DO re-declare the initialize method that creates the dispatcher thread
        void initialize();

        // TO-DO re-declare the list clients method that returns the connected clients information
        void listClients();

        // TO-DO re-declare the methods run by the dispatcher and worker threads
        void runDispatcher();
        void runWorker();
        
        // TO-DO re-declare graceful shutdown of server, threads and clients
        void shutdown();
};

#endif