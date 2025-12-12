#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

#include "ProcessingEngine.hpp"

unsigned short ProcessingEngine::datasetPathIndex = 0;

void ProcessingEngine::Mutex_Print(const int id, const char* message) const
{
    static std::mutex cout_mutex;
    std::lock_guard<std::mutex> guard(cout_mutex);

    if (id < 1)
    {
        std::cout << message << "\n";
    }
    else
    {
        std::cout << "Executing from the thread id: " << id << "\n\t";
        std::cout << "Message: " << message << "\n";
    }
}

void ProcessingEngine::FetchContentFromFile(const std::string& task, std::ifstream& inputFile, std::string& content)
{
    inputFile.clear();

    inputFile.open(task, std::ios::binary);

    if (!inputFile.is_open()) 
    {
        std::cerr << "Error opening file: " << task << std::endl;
        return;
    }

    content.clear();
    inputFile.seekg(0, std::ios::end);
    content.reserve(inputFile.tellg());
    inputFile.seekg(0, std::ios::beg);

    // Read the entire content of the file into a string
    content = { std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>() };

    inputFile.close();
}

void ProcessingEngine::CountWordFreq(const std::string& content, std::unordered_map<std::string, unsigned int>& wordFrequency, std::string& currentWord)
{
    currentWord.clear();
    wordFrequency.clear();

    // Process the content character by character
    for (char c : content)
    {
        if ((int)c > 0 && std::isalnum(c)) // c > 0 Extra test case for unidentified char.
        {
            currentWord += c;
            continue;
        }

        // If the current word is not empty, add it to the unordered_map
        if (!currentWord.empty())
        {
            // Increment the frequency of the current word in the unordered_map
            wordFrequency[currentWord]++;
            currentWord.clear(); // Clear the current word for the next one
        }
    }

    // If there's still a word remaining in the currentWord, add it to the unordered_map
    if (!currentWord.empty())
    {
        wordFrequency[currentWord]++;
    }
}

void ProcessingEngine::RunWorker(const int thread_id, bool& threadLaunched, queue_t& queue, std::weak_ptr<IndexStore> indexStore)
{
    //Mutex_Print(thread_id, "Thread launched");
 
    threadLaunched = !threadLaunched;

    int head;
    std::string task;

    std::ifstream inputFile;
    std::string content;
    std::string currentWord;
    std::unordered_map<std::string, unsigned int> wordFrequency;

    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(queue.mtx);
            head = queue.head++;
        }

        while (head >= queue.tail) 
        {
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(queue.mtx);
            task = queue.deque.front();
            queue.deque.pop_front();
        }

        if (task == "quit")
        {
            //Mutex_Print(thread_id, "Thread Exit");
            break;
        }
        
        // Do the main process below.
        FetchContentFromFile(task, inputFile, content);
        CountWordFreq(content, wordFrequency, currentWord);

        task = task.substr(ProcessingEngine::datasetPathIndex);

        const auto& sharedPtr = indexStore.lock();

        for (const auto& element : wordFrequency)
        {
            sharedPtr->insertIndex(element.first, task, element.second);
        }
    }
}

ProcessingEngine::ProcessingEngine(std::shared_ptr<IndexStore> store, const int num_workers) 
    : queue(), threads(), store(store)
{
    std::cout << std::endl;
    this->threads.reserve(num_workers);
    bool threadLaunched = false;

    for (int i = 0; i < num_workers; i++)
    {
        this->threads.emplace_back(std::thread(&ProcessingEngine::RunWorker, this, i + 1, std::ref(threadLaunched), std::ref(this->queue), this->store));

        while (!threadLaunched);

        threadLaunched = !threadLaunched;
    }
}

void ProcessingEngine::indexFiles(const std::string& directoryPath)
{
    ProcessingEngine::datasetPathIndex = directoryPath.length();

    std::cout << "\n";// << directoryPath << std::endl;

    // Split input directory work into all the threads.
    if (std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath))
    {
        for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(directoryPath))
        {
            if (dir_entry.is_directory())
            {
                continue;
            }

            std::lock_guard<std::mutex> lock(queue.mtx);
            queue.deque.emplace_back(dir_entry.path().string());
            queue.tail++;
        }

        while (queue.deque.size() != 0);
    }
}

//void ProcessingEngine::searchFiles(const std::vector<std::string>& wordsInput, std::vector<IndexStore::IndexingData>& results)
void ProcessingEngine::searchFiles(const std::vector<std::string>& wordsInput, IndexStore::IndexingData& results)
{
    //IndexStore::IndexingData temporary;
    std::unordered_map<std::string, unsigned int> result;
    std::unordered_map<std::string, unsigned int> temporary;
    bool found = false;

    results.list.clear();

    for (const std::string& word : wordsInput)
    {
        found = this->store->lookupIndex(word, temporary);

        if (found)
        {
            //if (results.size() > 0)
            if (result.size() > 0)
            {
                auto it = result.begin();
                int i = 0;
                while (i < result.size()) 
                {
                    it = result.begin();
                    advance(it, i);

                    if (temporary.find(it->first) != temporary.end()) 
                    {
                        result[it->first] += temporary[it->first];
                        ++i;
                    }
                    else 
                    {
                        result.erase(it);
                    }
                }
            }
            else
            {
                //results.push_back(temporary);
                //result = std::move(temporary);

                for (const auto& pair : temporary)
                {
                    //result.list[pair.second] = pair.first;
                    result[pair.first] = pair.second;
                }
            }

            //temporary.list.clear();
            temporary.clear();
        }

        found = false;
    }

    for (const auto& pair : result)
    {
        results.list[pair.second] = pair.first;
    }
}

void ProcessingEngine::stopWorkers() 
{
    for (int i = 0; i < this->threads.size(); i++) 
    {
        std::lock_guard<std::mutex> lock(queue.mtx);
        queue.deque.push_back("quit");
        queue.tail++;
    }

    for (int i = 0; i < this->threads.size(); i++)
    {
        this->threads[i].join();
    }
}