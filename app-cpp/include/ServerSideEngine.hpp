#ifndef SERVER_SIDE_ENGINE_H
#define SERVER_SIDE_ENGINE_H

#include <memory>
#include <thread>
#include <vector>
#include <unordered_map>
#include <deque>
#include <mutex>

#include "IndexStore.hpp"

struct queue_t
{
    std::deque<std::string> deque;
    std::mutex mtx;
    int head;
    int tail;

    //big 4
    queue_t()
        : deque(), mtx(), head(0), tail(0)
    {
    }
    ~queue_t() = default;

    queue_t(const queue_t& cpy) = delete;
    queue_t& operator=(const queue_t& cpy) = delete;
};

class ServerSideEngine {
    // TO-DO keep track of the dispatcher thread, worker threads and the connected clients
    // TO-DO keep track of the index store

    struct ClientData
    {
        std::string clientAddress;
        int         clientPort;
        int         clientID;
        //bool        isConnected;
        //static unsigned int threadLocalID;

        ClientData() = default;
        ClientData(const ClientData& cpy) = default;

        ClientData& operator=(const ClientData& cpy) = default;
        ~ClientData() = default;
        ClientData(std::string _address, int _port, int _clientID)
            :clientAddress(_address), clientPort(_port), clientID(_clientID)
        {
        }
        /*ClientData(std::string _address, int _port, bool _isConnected = true)
            :clientAddress(_address), clientPort(_port), isConnected(_isConnected)
        {
        }*/
    };

    queue_t                                 queue;
    //std::vector<std::thread>              threads;
    std::unordered_map<int, ClientData>     clientList;
    //std::vector<bool*>                      threadsBoolStorage;
    std::shared_ptr<IndexStore>             store;
    std::string                             serverAddress;
    std::string                             serverPort;
    std::thread                             dispatcherThread;
    const int                               maxNumConnections;
    bool                                    quitCommandExecuted;

    public:
        // constructor
        ServerSideEngine(std::shared_ptr<IndexStore> store);

        // default virtual destructor
        virtual ~ServerSideEngine() = default;

        // TO-DO re-declare the initialize method that creates the dispatcher thread
        void initialize(const std::string address, const  std::string port);

        // TO-DO re-declare the list clients method that returns the connected clients information
        void listClients();

        // TO-DO re-declare the methods run by the dispatcher and worker threads
        void runDispatcherWithoutQueue(const unsigned int thread_id, bool& threadLaunched);
        void runSubWorker(const unsigned int thread_id, bool& threadLaunched, const int sock);

        void PerformWordsSearch(const char* wordsInput, const int numBytes, const int sock);

        //void runDispatcher(const unsigned int thread_id, bool& threadLaunched, queue_t& queue);
        //void runWorker(const unsigned int thread_id, bool& threadLaunched, queue_t& queue);
        
        void OnServerQuitEvent();
        void SendQuitEventAllClients();

        // TO-DO re-declare graceful shutdown of server, threads and clients
        void shutdown();

        static const size_t MAX_BUFFER_SIZE = 2048;
};

#endif