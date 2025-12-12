#ifndef CLIENT_SIDE_ENGINE_H
#define CLIENT_SIDE_ENGINE_H

class ClientSideEngine {
    // TO-DO keep track of the connection

    public:
        // constructor
        ClientSideEngine();

        // default virtual destructor
        virtual ~ClientSideEngine() = default;

        // TO-DO re-declare index files and search files methods
        void indexFiles();
        void searchFiles();
};

#endif