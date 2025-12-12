#include "ClientSideEngine.hpp"

ClientSideEngine::ClientSideEngine() {
    // TO-DO add information to keep track of socket connection
}

void ClientSideEngine::indexFiles() {
    // TO-DO crawl the files from the input folder
    // for each file read and count the words and send the counted words to the server
}

void ClientSideEngine::searchFiles() {
    // TO-DO extract the terms from the query
    // for each term contact the server to retrieve the list of documents that contain the word
    // combine the results of a multi-term query
    // return top 10 results
}

void ClientSideEngine::openConnection() {
    // TO-DO create a new TCP/IP connection to the server
}

void ClientSideEngine::closeConnection() {
    // TO-DO close the connection to the server
}