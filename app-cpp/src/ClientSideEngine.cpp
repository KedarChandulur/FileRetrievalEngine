#include <filesystem>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
}

#include "Utilities.hpp"
#include "ClientSideEngine.hpp"

unsigned short ClientSideEngine::datasetPathIndex = 0;

ClientSideEngine::ClientSideEngine()
    : queue(), localWorkerThreads(), clientAddress(), clientPort(), buf(), clientID(), searchBuffer(), dataReadThread(), sockfd(-1), numLocalWorkerThreadsCount(1), dataSuccessfullySent(false), dataSuccessfullyReceived(false), isClientConnectedToServer(false)
{
    // TO-DO add information to keep track of socket connection
}

void ClientSideEngine::InitializeLocalThreads(const int _minFolder, const int _maxFolder, const int _numWorkers)
{
    this->numLocalWorkerThreadsCount = _numWorkers;
    //this->localWorkerThreads.reserve(_numWorkers + 1);
    this->localWorkerThreads.reserve(_numWorkers);

    this->minFolder = _minFolder;
    this->maxFolder = _maxFolder;

    bool threadLaunched = false;

    //this->localWorkerThreads.emplace_back(std::thread(&ClientSideEngine::GetDataFromServer, this, 0, std::ref(threadLaunched)));
    dataReadThread = std::thread(&ClientSideEngine::GetDataFromServer, this, 0, std::ref(threadLaunched));
    dataReadThread.detach();

    while (!threadLaunched);
}

void ClientSideEngine::indexFiles(const std::string& directoryPath)
{
    // TO-DO crawl the files from the input folder
    // for each file read and count the words and send the counted words to the server

    ClientSideEngine::datasetPathIndex = directoryPath.find("Dataset");
    ClientSideEngine::datasetPathIndex += 8;

    std::string subDirectoryPath;

    for (int i = this->minFolder; i <= this->maxFolder; ++i)
    {
        subDirectoryPath.clear();
        subDirectoryPath = directoryPath.substr(0, ClientSideEngine::datasetPathIndex) + "/" + "folder" + std::to_string(i) + "/";
        std::cout << subDirectoryPath << std::endl;

        QueueFiles(subDirectoryPath);
    }

    Utilites::Mutex_Print(-1, "Processing Indexing Request: \n", false);

    while (queue.deque.size() != 0)
    {
        if (!isClientConnectedToServer || sockfd < 0)
        {
            std::cout << "Client disconnected from the server\n";
            //std::cout << "!isClientConnectedToServer: " << (bool)isClientConnectedToServer << std::endl;
            std::cout << "sockfd: " << sockfd << std::endl;
            break;
        }
    }

    char* buf = new char[30];

    memset(buf, 0, 30);
    memcpy(buf, "Finished indexing client side.", 30);

    //if (send(sockfd, buf.c_str(), buf.size(), 0) == -1)
    if (send(sockfd, buf, 30, 0) == -1)
    {
        std::cerr << "Error sending Finished indexing client side message." << std::endl;
        std::cerr << "Aborting the connection!" << std::endl;

        delete[] buf;

        this->closeConnection();
        return;
    }

    std::cout << "Finished indexing client side.\n";
    delete[] buf;
}

void ClientSideEngine::QueueFiles(const std::string& _subDirectoryPath)
{
    // Split input directory work into all the threads.
    if (std::filesystem::exists(_subDirectoryPath))
    {
        for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(_subDirectoryPath))
        {
            if (dir_entry.is_directory())
            {
                continue;
            }

            std::lock_guard<std::mutex> lock(queue.mtx);
            queue.deque.emplace_back(dir_entry.path().string());
            queue.tail++;
        }
    }
}

void ClientSideEngine::searchFiles(const std::string& _query, IndexStore::IndexingData& results)
{
    //std::cout << "Search query: " << _query << std::endl;

    char* buf = new char[_query.size() + 1];
    memset(buf, 0, _query.size() + 1);
    memcpy(buf, _query.c_str(), _query.size());

    //std::cout << buf << std::endl;

    if (send(this->sockfd, buf, strlen(buf), 0) == -1)
    {
        std::cerr << "Error sending search data!" << std::endl;
        std::cerr << "Aborting the connection!" << std::endl;

        delete[] buf;
        this->closeConnection();
        return;
    }

    delete[] buf;

    Utilites::Mutex_Print(-1, "Processing Search Request: \n", false);

    while (searchBuffer.size() == 0)
    {
        if (!isClientConnectedToServer || sockfd < 0)
        {
            std::cout << "Client disconnected from the server\n";
            //std::cout << "!isClientConnectedToServer: " << (bool)isClientConnectedToServer << std::endl;
            std::cout << "sockfd: " << sockfd << std::endl;
            break;
        }
    }

    results.list.clear();
    //std::cout << searchBuffer << std::endl;

    if (searchBuffer.size() == 21 && searchBuffer == "Success-Search: Empty")
    {
        //std::cout << "Search was empty.\n";
        searchBuffer.clear();
        return;
    }

    if (searchBuffer.size() >= 15 && searchBuffer.substr(0, 15) == "Success-Search:")
    {
        std::istringstream iss;
        std::string word1;
        std::string word2;
        unsigned num1;

        iss.str("");
        iss.clear();
        iss.str(searchBuffer);

        word1.clear();
        word2.clear();

        iss >> word1;

        //std::cout << word1 << std::endl;

        while (iss >> word1 >> word2)
        {
            num1 = std::stoul(word1);
            results.list[num1] = word2;
        }

        searchBuffer.clear();
        return;
    }

    if (searchBuffer.size() == 24 && searchBuffer == "Success-Search: Overflow")
    {
        std::cout << "Search buffer overflowed.\n";
        searchBuffer.clear();
        return;
    }

    std::cout << "Error: Client search hit the end of the search function, something didn't pass correctly.";

    if (searchBuffer.size() < 22)
    {
        std::cout << searchBuffer << std::endl;
    }
    else
    {
        std::cout << searchBuffer.size() << std::endl;
    }
}

void ClientSideEngine::searchFiles(const std::vector<std::string>& wordsInput, IndexStore::IndexingData& results)
{
    // TO-DO extract the terms from the query
    // for each term contact the server to retrieve the list of documents that contain the word
    // combine the results of a multi-term query
    // return top 10 results

    std::unordered_map<std::string, unsigned int> result;
    std::unordered_map<std::string, unsigned int> temporary;
    bool found = false;

    results.list.clear();

    for (const std::string& word : wordsInput)
    {
              /*
              Look Up in Server here
              */
        //found = this->store->lookupIndex(word, temporary);

        //if (found)
        //{
              /*
              Combing every result from the server below.
              */
        //    
        //    //if (results.size() > 0)
        //    if (result.size() > 0)
        //    {
        //        auto it = result.begin();
        //        int i = 0;
        //        while (i < result.size())
        //        {
        //            it = result.begin();
        //            advance(it, i);

        //            if (temporary.find(it->first) != temporary.end())
        //            {
        //                result[it->first] += temporary[it->first];
        //                ++i;
        //            }
        //            else
        //            {
        //                result.erase(it);
        //            }
        //        }
        //    }
        //    else
        //    {
        //        //results.push_back(temporary);
        //        //result = std::move(temporary);

        //        for (const auto& pair : temporary)
        //        {
        //            //result.list[pair.second] = pair.first;
        //            result[pair.first] = pair.second;
        //        }
        //    }

        //    //temporary.list.clear();
        //    temporary.clear();
        //}

        //found = false;
    }

    for (const auto& pair : result)
    {
        results.list[pair.second] = pair.first;
    }
}

void ClientSideEngine::openConnection(const std::string _address, const std::string _port) 
{
    // TO-DO create a new TCP/IP connection to the server

    assert(!_address.empty());
    assert(!_port.empty());

    this->clientAddress = _address;
    this->clientPort = _port;

    struct addrinfo hints;
    struct addrinfo* servinfo;
    struct addrinfo* p;
    char addr[INET6_ADDRSTRLEN];
    int rc;

    //char* buf = new char[MAX_BUFFER_SIZE];
    //int numBytes;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rc = getaddrinfo(this->clientAddress.c_str(), this->clientPort.c_str(), &hints, &servinfo)) != 0)
    {
        std::cerr << "Could not get address information!" << std::endl;
        return;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            std::cerr << "Could not create socket!" << std::endl;
            this->InvalidateSocket();
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            std::cerr << "Could not connect to server!" << std::endl;
            this->InvalidateSocket();
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        std::cerr << "Server failed to bind!" << std::endl;
        return;
    }

    if (((struct sockaddr*)&p)->sa_family == AF_INET)
    {
        inet_ntop(p->ai_family, &(((struct sockaddr_in*)&p->ai_addr)->sin_addr), addr, sizeof(addr));
    }
    else
    {
        inet_ntop(p->ai_family, &(((struct sockaddr_in6*)&p->ai_addr)->sin6_addr), addr, sizeof(addr));
    }

    //std::cout << "Client connected to " << this->clientAddress << " Server!" << std::endl;
    Utilites::Mutex_Print(-1, "Client connected to ", false);
    Utilites::Mutex_Print(-1, this->clientAddress.c_str(), false);
    Utilites::Mutex_Print(-1, " Server!");

    freeaddrinfo(servinfo);

    isClientConnectedToServer = true;

    std::lock_guard<std::mutex> lock(queue.mtx);
    queue.deque.emplace_back("isClientConnectedToServer");
    queue.tail++;
}

void ClientSideEngine::closeConnection() 
{
    // TO-DO close the connection to the server

    if (isClientConnectedToServer)
    {
        char* buf = new char[5];
        memset(buf, 0, 5);
        strcpy(buf, "quit");

        if (send(this->sockfd, buf, strlen(buf), 0) == -1)
        {
            if(isClientConnectedToServer)
            {
                //std::cerr << "Did you connect to the server before quiting?-" << std::endl;
                //std::cerr << this->sockfd << std::endl;
            }
        }

        delete[] buf;
    }

    Exit();
}

void ClientSideEngine::Exit()
{
    if (isClientConnectedToServer)
    {
        close(sockfd);
        sockfd = -1;
    }

    StopLocalWorkers();

    isClientConnectedToServer = false;
}

//---------------------------------------- Old Methods Below

void ClientSideEngine::StopLocalWorkers()
{
    for (int i = 0; i < this->localWorkerThreads.size() ; i++)
    {
        std::lock_guard<std::mutex> lock(queue.mtx);
        queue.deque.push_back("quit");
        queue.tail++;
    }

    for (int i = 0; i < this->localWorkerThreads.size(); i++)
    {
        if(this->localWorkerThreads[i].joinable())
        {
            this->localWorkerThreads[i].join();
        }
    }
}

void ClientSideEngine::FetchContentFromFile(const std::string& task, std::ifstream& inputFile, std::string& content)
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

void ClientSideEngine::CountWordFreq(const std::string& content, std::unordered_map<std::string, unsigned int>& wordFrequency, std::string& currentWord)
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

void ClientSideEngine::RunLocalWorker(const int thread_id, bool& threadLaunched, queue_t& queue)
{
    //Mutex_Print(thread_id, "Thread launched");

    threadLaunched = !threadLaunched;

    int head;
    std::string task;

    std::ifstream inputFile;
    std::string content;
    std::string currentWord;
    std::unordered_map<std::string, unsigned int> wordFrequency;


    char* bufTesting = new char[MAX_BUFFER_SIZE];
    //std::string buf;
    buf.reserve(MAX_BUFFER_SIZE - 1);

    int numBytes;
    std::string temporaryBuf;

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
            //Utilites::Mutex_Print(thread_id, "Thread Exit");
            break;
        }

        //if(task.size() >= )
        // Do the main process below.
        FetchContentFromFile(task, inputFile, content);
        CountWordFreq(content, wordFrequency, currentWord);

        task = task.substr(ClientSideEngine::datasetPathIndex + 1);
        task = clientID + task;
        //std::cout << "task substr:    " << task << "\n";

        // Below send word count to the server for indexing
        temporaryBuf.clear();
        buf.clear();
        buf += "Index: ";

        for (const auto& element : wordFrequency)
        {
            temporaryBuf.clear();
            temporaryBuf += element.first + " " + task + " " + std::to_string(element.second) + " ";

            if (buf.size() + temporaryBuf.size() < MAX_BUFFER_SIZE)
            {
                //strcat(bufTesting, temporaryBuf.c_str());
                buf.append(temporaryBuf);
            }
            else
            {
                //std::cout << "Full Data Size: " << buf.size() + temporaryBuf.size() << std::endl;

                memset(bufTesting, 0, MAX_BUFFER_SIZE);
                memcpy(bufTesting, buf.c_str(), buf.size());

                //if (send(sockfd, buf.c_str(), buf.size(), 0) == -1)
                if (send(sockfd, bufTesting, buf.size(), 0) == -1)
                {
                    std::cerr << "Error sending index data!" << std::endl;
                    std::cerr << "Aborting the connection!" << std::endl;

                    delete[] bufTesting;

                    this->closeConnection();
                    break;
                }

                //std::cout << "Data Transfer to Server Successful\n";
                dataSuccessfullySent = true;

                memset(bufTesting, 0, MAX_BUFFER_SIZE);
                //buf.clear();

                //if ((numBytes = recv(sockfd, buf.data(), MAX_BUFFER_SIZE - 1, 0)) == -1)
                /*if ((numBytes = recv(sockfd, bufTesting, MAX_BUFFER_SIZE - 1, 0)) == -1)
                {
                    std::cerr << "Error receiving result, after sending indexing data!" << std::endl;
                    std::cerr << "Aborting the connection!" << std::endl;

                    delete[] bufTesting;

                    this->closeConnection();
                    continue;
                }*/

                while(dataSuccessfullyReceived == false);

                //std::cout << "BOOL dataSuccessfullyReceived changed: " << dataSuccessfullyReceived << "\n";
                //std::cout << "Data Received From Server Successful\n";

                //std::cout << "bufTesting: " << strlen(bufTesting) << std::endl;

                /*memset(bufTesting, 0, MAX_BUFFER_SIZE);
                strcat(bufTesting, temporaryBuf.c_str());*/

                dataSuccessfullyReceived = false;

                buf.clear();
                buf.append("Index: " + temporaryBuf);

                //std::cout << "REMOVE BREAK ON LINE 516.\n";
                //break;
            }
        }

        memset(bufTesting, 0, MAX_BUFFER_SIZE);
    }

    delete[] bufTesting;
}

void ClientSideEngine::GetDataFromServer(const int thread_id, bool& threadLaunched)
{
    threadLaunched = !threadLaunched;

    char* bufTesting = new char[MAX_BUFFER_SIZE];
    int numBytes;

    int head;
    std::string task;

    while (isClientConnectedToServer == false)
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

        //std::cout << "Found a task: " << task << std::endl;

        if (task == "quit")
        {
            Utilites::Mutex_Print(thread_id, "Thread Exit");
            break;
        }

        if (task == "isClientConnectedToServer")
        {
            //Local Threads Launch
            bool threadLaunched = false;

            for (int i = 0; i < this->numLocalWorkerThreadsCount; i++)
            {
                this->localWorkerThreads.emplace_back(std::thread(&ClientSideEngine::RunLocalWorker, this, i + 1, std::ref(threadLaunched), std::ref(this->queue)));

                while (!threadLaunched);

                threadLaunched = !threadLaunched;
            }
            //Local Threads Launch
            continue;
        }
    }

    if (isClientConnectedToServer)
    {
        //std::cout << "Client Connected To The Server." << std::endl;

        while (true)
        {
            /*while (dataSuccessfullySent == false);

            std::cout << "BOOL dataSuccessfullySent changed: " << dataSuccessfullySent << "\n";

            dataSuccessfullySent = false;*/

            memset(bufTesting, 0, MAX_BUFFER_SIZE);

            if ((numBytes = recv(sockfd, bufTesting, MAX_BUFFER_SIZE - 1, 0)) == -1)  // MESSAGE
                //if ((numBytes = recv(sockfd, buf.data(), MAX_BUFFER_SIZE - 1, 0)) == -1)
            {
                if (isClientConnectedToServer)
                {
                    //std::cerr << "Error receiving result, after sending indexing data!" << std::endl;
                    //std::cerr << "Aborting the connection!" << std::endl;
                }

                delete[] bufTesting;
                bufTesting = nullptr;

                this->closeConnection();
                break;
            }

            if (strlen(bufTesting) == 7 && (strncmp(bufTesting, "Success", 7) == 0))
            {
                dataSuccessfullyReceived = true;

                //std::cout << "Success Message Received\n";
                //std::cout << "BOOL dataSuccessfullyReceived changed: " << dataSuccessfullyReceived << "\n";
                //std::cout << "Successful data received from server\n";
                //std::cout << "Inside success buftesting: " << bufTesting << std::endl;

                memset(bufTesting, 0, MAX_BUFFER_SIZE);

                continue;
            }

            if (strlen(bufTesting) >= 14 && (strncmp(bufTesting, "Success-Search", 14) == 0))
            {
                //std::cout << "Success-Search Message Received\n";
                //std::cout << "Successful data received from server\n";
                //std::cout << bufTesting << std::endl;

                searchBuffer = bufTesting;

                //std::cout << searchBuffer << std::endl;

                memset(bufTesting, 0, MAX_BUFFER_SIZE);

                continue;
            }

            if (strlen(bufTesting) == 15 && (strncmp(bufTesting, "Server-ShutDown", 15) == 0))
            {
                if (isClientConnectedToServer)
                {
                    char* buf = new char[27];
                    memset(buf, 0, 27);
                    strcpy(buf, "Client-ShutDown-Successful");

                    if (send(this->sockfd, buf, strlen(buf), 0) == -1)
                    {
                        std::cout << "Something went wrong when sending Server-ShutDown signal to server." << std::endl;
                    }

                    delete[] buf;
                }

                //{
                //std::lock_guard<std::mutex> lock(sockfdmtx);
                //isClientConnectedToServer = false;
                //}

                /*InvalidateSocket();
                StopLocalWorkers();

                dataReadThread.join();*/

                this->Exit();

                Utilites::Mutex_Print(-1, "Server Disconnected and went offline.");
                Utilites::Mutex_Print(-1, "Press quit command to quit your application and Restart it when the server is up again");

                break;
            }

            if (strlen(bufTesting) >= 18 && (strncmp(bufTesting, "ClientName: client", 18) == 0))
            {
                this->clientID.clear();

                if (bufTesting[18] != '0')
                {
                    this->clientID += bufTesting[18];
                }

                this->clientID += bufTesting[19];
                this->clientID += ":";

                //this->clientID.assign(bufTesting + 18, strlen(bufTesting) - 18);

                //Utilites::Mutex_Print(-1, "Printing Received Client ID ", false);
                //Utilites::Mutex_Print(-1, this->clientID.c_str());
                //Utilites::Mutex_Print(-1, "", true);
                //Utilites::Mutex_Print(-1, "> ", false);
                //std::cout << "Printing Received Client ID: " << this->clientID << std::endl;

                memset(bufTesting, 0, MAX_BUFFER_SIZE);
                continue;
            }

            //buf.clear();
            if(strlen(bufTesting) > 0)
            {
                std::cout << "Some unknown data received from the server: " << bufTesting << "\t" << strlen(bufTesting) << "\n";
            }

            memset(bufTesting, 0, MAX_BUFFER_SIZE);
        }
    }
    else
    {
        std::cout << "Did you forgot to connnect to server?" << std::endl;
        assert(false);
    }

    delete[] bufTesting;
    bufTesting = nullptr;
}

void ClientSideEngine::InvalidateSocket()
{
    if (sockfd > -1)
    {
        std::cout << "Invalidate calling on socket: " << sockfd << std::endl;
        close(sockfd);
        sockfd = -1;
    }
}