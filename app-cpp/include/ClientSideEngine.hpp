#ifndef CLIENT_SIDE_ENGINE_H
#define CLIENT_SIDE_ENGINE_H

#include <deque>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <IndexStore.hpp>

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

class ClientSideEngine
{
    // TO-DO keep track of the connection

    queue_t                     queue;
    std::vector<std::thread>    localWorkerThreads;
    std::string                 clientAddress;
    std::string                 clientPort;
    std::string                 buf;
    std::string                 clientID;
    std::string                 searchBuffer;
    std::thread                 dataReadThread;
    int                         sockfd;
    int                         numLocalWorkerThreadsCount;
    int                         minFolder;
    int                         maxFolder;
    bool                        dataSuccessfullySent;
    bool                        dataSuccessfullyReceived;
    bool                        isClientConnectedToServer;

    public:
        // constructor
        ClientSideEngine();

        // default virtual destructor
        virtual ~ClientSideEngine() = default;

        void InitializeLocalThreads(const int _minFolder, const int _maxFolder, const int numWorkers = 1);

        // TO-DO re-declare index files and search files methods
        void indexFiles(const std::string& directoryPath);
        void QueueFiles(const std::string& _subDirectoryPath);
        void searchFiles(const std::string& _query, IndexStore::IndexingData& results);
        void searchFiles(const std::vector<std::string>& wordsInput, IndexStore::IndexingData& results);
        
        // TO-DO re-declare connect to and disconnect from the server
        void openConnection(const std::string address, const std::string port);
        void closeConnection();

        //---------------------------------------- Old Methods Below

        void FetchContentFromFile(const std::string& task, std::ifstream& inputFile, std::string& content);

        void CountWordFreq(const std::string& content, std::unordered_map<std::string, unsigned int>& wordFrequency, std::string& currentWord);

        void SendDataToServer(const std::string& other);

        void RunLocalWorker(const int thread_id, bool& threadLaunched, queue_t& queue);

        void GetDataFromServer(const int thread_id, bool& threadLaunched);

        void InvalidateSocket();

        void Exit();

        void StopLocalWorkers();

        static unsigned short datasetPathIndex; // Limit to 10 entries
        static const size_t MAX_BUFFER_SIZE = 2048;
};

#endif