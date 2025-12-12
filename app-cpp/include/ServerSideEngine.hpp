#ifndef SERVER_SIDE_ENGINE_H
#define SERVER_SIDE_ENGINE_H

#include <memory>

#include "IndexStore.hpp"

class ServerSideEngine {
    // TO-DO keep track of the index store
    std::shared_ptr<IndexStore> store;

    public:
        // constructor
        ServerSideEngine(std::shared_ptr<IndexStore> store);

        // default virtual destructor
        virtual ~ServerSideEngine() = default;

        // TO-DO create and register the gRPC service
        void initialize();

        // TO-DO re-declare the methods in order to perfom indexing and search
        void index();
        void search();
        
        // TO-DO re-declare graceful shutdown of the gRPC service
        void shutdown();
};

#endif