#ifndef PROCESSING_ENGINE_H
#define PROCESSING_ENGINE_H

#include <memory>
#include <deque>
#include <vector>
#include <thread>
#include <filesystem>

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

class ProcessingEngine 
{
    // TO-DO Keep track of index and search workers
    queue_t                     queue;
    std::vector<std::thread>    threads;
    std::shared_ptr<IndexStore> store;

    public:
        // constructor
        ProcessingEngine(std::shared_ptr<IndexStore> store, const int num_workers);

        // default virtual destructor
        virtual ~ProcessingEngine() = default;

        // TO-DO re-declare index files and search files methods
        void indexFiles(const std::string& directoryPath);
        //void searchFiles(const std::vector<std::string>& wordsInput, std::vector<IndexStore::IndexingData>& results);
        void searchFiles(const std::vector<std::string>& wordsInput, IndexStore::IndexingData& result);
        
        // gracefully stop index and search workers
        void stopWorkers();

        void Mutex_Print(const int id, const char* message) const;

        void FetchContentFromFile(const std::string& task, std::ifstream& inputFile, std::string& content);
        void CountWordFreq(const std::string& content, std::unordered_map<std::string, unsigned int>& wordFrequency, std::string& currentWord);

        void RunWorker(const int thread_id, bool& threadLaunched, queue_t& queue, std::weak_ptr<IndexStore> indexStore);

        static unsigned short datasetPathIndex; // Limit to 10 entries
};

#endif