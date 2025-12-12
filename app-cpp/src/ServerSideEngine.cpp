#include "ServerSideEngine.hpp"

ServerSideEngine::ServerSideEngine(std::shared_ptr<IndexStore> store) : store(store) {
    // TO-DO implement constructor
}

void ServerSideEngine::initialize() {
    // TO-DO create and register the gRPC service
}

void ServerSideEngine::index() {
    // TO-DO implement the server-side indexing operation (only what the server would process when indexing)
}

void ServerSideEngine::search() {
    // TO-DO implement the server-side search operation (only what the server would process when searching)
}

void ServerSideEngine::shutdown() {
    // TO-DO implement graceful shutdown of the gRPC service
}