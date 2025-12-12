#include "ServerSideEngine.hpp"

ServerSideEngine::ServerSideEngine(std::shared_ptr<IndexStore> store) : store(store) {
    // TO-DO implement constructor
}

void ServerSideEngine::initialize() {
    // TO-DO create one dispatcher thread that runs the runDispatcher method
}

void ServerSideEngine::listClients() {
    // TO-DO get the connected clients information and return the information
}

void ServerSideEngine::runDispatcher() {
    // TO-DO create the server socket and listen for and accept new connections
    // HINT each new connection gets managed by a different worker thread -> create new worker thread on new connection
}

void ServerSideEngine::runWorker() {
    // TO-DO receive index and search commands from the client until the client disconnects
}

void ServerSideEngine::shutdown() {
    // TO-DO join the dispatcher and worker threads
}