#include "ClientSideEngine.hpp"

ClientSideEngine::ClientSideEngine() {
    // TO-DO create the gRPC client side stub
}

void ClientSideEngine::indexFiles() {
    // TO-DO crawl the files from the input folder
    // for each file read and count the words and send the counted words to the server by calling the client stub
}

void ClientSideEngine::searchFiles() {
    // TO-DO extract the terms from the query
    // for each term retrieve from the server the list of documents that contain the word by calling the client stub
    // combine the results of a multi-term query
    // return top 10 results
}