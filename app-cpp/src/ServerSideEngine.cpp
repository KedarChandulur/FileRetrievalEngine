#include <iostream>
#include <cassert>
#include <string>
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
#include "ServerSideEngine.hpp"

//unsigned int ServerSideEngine::ClientData::threadLocalID = 1;

ServerSideEngine::ServerSideEngine(std::shared_ptr<IndexStore> store)
    //: queue(), dispatcherThread(), clientList(), threadsBoolStorage(), store(store), serverAddress(""), serverPort(""), maxNumConnections(16), quitCommandExecuted(false)
    : queue(), clientList(), store(store), serverAddress(""), serverPort(""), dispatcherThread(), maxNumConnections(16), quitCommandExecuted(false)
{
    // TO-DO implement constructor
    //threads.reserve(maxNumConnections + 1);
    //threadsBoolStorage.reserve(maxNumConnections);
    clientList.reserve(maxNumConnections + 1);
}

void ServerSideEngine::listClients()
{
    // TO-DO get the connected clients information and return the information
    if (clientList.size() < 1)
    {
        std::cout << "No clients connected, waiting for clients to connect" << std::endl;
        return;
    }
    
    //size_t i = 0;

    for (auto& value : clientList)
    {
        // client1: 127.0.0.1 5746
        std::cout << "client" << value.second.clientID << ": " << value.second.clientAddress << " " << value.second.clientPort << std::endl;
    }
}

void ServerSideEngine::initialize(const std::string _address, const std::string _port)
{
    // TO-DO create one dispatcher thread that runs the runDispatcher method

    bool threadLaunched = false;

    assert(!_address.empty());
    assert(!_port.empty());

    this->serverAddress = _address;
    this->serverPort = _port;

    assert(maxNumConnections == 16);

    //threads.emplace_back(std::thread(&ServerSideEngine::runDispatcher, this, 1, std::ref(threadLaunched), std::ref(this->queue)));
    // 
    //threads.emplace_back(std::thread(&ServerSideEngine::runDispatcherWithoutQueue, this, 1, std::ref(threadLaunched)));

    dispatcherThread = std::thread(&ServerSideEngine::runDispatcherWithoutQueue, this, 1, std::ref(threadLaunched));
    while (!threadLaunched);
}

//void ServerSideEngine::runDispatcher(const unsigned int thread_id, bool& threadLaunched, queue_t& queue)
//{
//    // TO-DO create the server socket and listen for and accept new connections
//    // HINT each new connection gets managed by a different worker thread -> create new worker thread on new connection
//
//    threadLaunched = true;
//
//    bool dispacterLocal_ThreadLauched = false;
//    unsigned int threadLocalID = 1;
//
//    int head;
//    std::string task;
//
//    int sockfd, client_sock;
//    struct addrinfo hints;
//    struct addrinfo* servinfo;
//    struct addrinfo* p;
//    struct sockaddr_storage clt_addr;
//    socklen_t sin_size;
//    char addr[INET6_ADDRSTRLEN];
//    int yes = 1;
//    int rc;
//    int i = maxNumConnections;
//
//    char* bufTesting = new char[MAX_BUFFER_SIZE];
//
//    std::vector<std::thread> threads;
//
//    //std::vector<int> clientSocketsList;
//    //clientSocketsList.reserve(maxNumConnections);
//
//    memset(&hints, 0, sizeof(hints));
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_flags = AI_PASSIVE;
//
//    if ((rc = getaddrinfo(this->serverAddress.c_str(), this->serverPort.c_str(), &hints, &servinfo)) != 0)
//    {
//        std::cerr << "Could not get address information!" << std::endl;
//        return;
//    }
//
//    for (p = servinfo; p != NULL; p = p->ai_next)
//    {
//        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
//        {
//            std::cerr << "Could not create socket!" << std::endl;
//            continue;
//        }
//
//        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
//        {
//            std::cerr << "Could not configure socket!" << std::endl;
//            return;
//        }
//
//        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
//        {
//            close(sockfd);
//            std::cerr << "Could not bind socket!" << std::endl;
//            continue;
//        }
//
//        break;
//    }
//
//    freeaddrinfo(servinfo);
//
//    if (p == NULL)
//    {
//        std::cerr << "Server failed to bind!\np is NULL" << std::endl;
//        return;
//    }
//
//    if (listen(sockfd, this->maxNumConnections) == -1)
//    {
//        std::cerr << "Server failed to listen!\nMaximum number of connections before even listening" << std::endl;
//        return;
//    }
//
//    std::cout << "Server started and waiting for connections!" << std::endl;
//
//    while (!quitCommandExecuted)
//    {
//        sin_size = sizeof(clt_addr);
//        client_sock = accept(sockfd, (struct sockaddr*)&clt_addr, &sin_size);
//
//        if (client_sock == -1)
//        {
//            std::cerr << "Server failed to accept client!\nclient_sock value is -1" << std::endl;
//            continue;
//        }
//
//        if (((struct sockaddr*)&clt_addr)->sa_family == AF_INET)
//        {
//            inet_ntop(clt_addr.ss_family, &(((struct sockaddr_in*)&clt_addr)->sin_addr), addr, sizeof(addr));
//        }
//        else
//        {
//            inet_ntop(clt_addr.ss_family, &(((struct sockaddr_in6*)&clt_addr)->sin6_addr), addr, sizeof(addr));
//        }
//
//        if (!quitCommandExecuted)
//        {
//            std::cout << "Server got connection from " << addr << "!" << std::endl;
//
//            clientList[client_sock] = addr;
//            //clientSocketsList.emplace_back(client_sock);
//            threads.emplace_back(std::thread(&ServerSideEngine::runWorker, this, ++threadLocalID, std::ref(dispacterLocal_ThreadLauched), std::ref(this->queue)));
//
//            while (!dispacterLocal_ThreadLauched);
//
//            dispacterLocal_ThreadLauched = !dispacterLocal_ThreadLauched;
//
//            i--;
//        }
//    }
//
//    for (int i = 1; i < threads.size(); ++i)
//    {
//        std::lock_guard<std::mutex> lock(queue.mtx);
//        queue.deque.push_back("quit");
//        queue.tail++;
//    }
//
//    while (queue.deque.size() != 0);
//
//    for (auto value : clientList)
//    {
//        close(value.first);
//    }
//
//    close(sockfd);
//
//    delete[] bufTesting;
//
//    for (int i = 0; i < threads.size(); ++i)
//    {
//        threads[i].join();
//    }
//}

//void ServerSideEngine::runWorker(const unsigned int thread_id, bool& threadLaunched, queue_t& queue)
//{
//    // TO-DO receive index and search commands from the client until the client disconnects
//
//    threadLaunched = true;
//
//    int head;
//    std::string task;
//
//    while (true)
//    {
//        assert(!this->serverAddress.empty());
//        assert(!this->serverPort.empty());
//
//        //---------------------------------------- Old Code
//        {
//            std::lock_guard<std::mutex> lock(queue.mtx);
//            head = queue.head++;
//        }
//
//        while (head >= queue.tail)
//        {
//            continue;
//        }
//
//        {
//            std::lock_guard<std::mutex> lock(queue.mtx);
//            task = queue.deque.front();
//            queue.deque.pop_front();
//        }
//
//        if (task == "quit")
//        {
//            break;
//        }
//        //---------------------------------------- Old Code
//
//
//    }
//}

void ServerSideEngine::runDispatcherWithoutQueue(const unsigned int thread_id, bool& threadLaunched)
{
    // TO-DO create the server socket and listen for and accept new connections
    // HINT each new connection gets managed by a different worker thread -> create new worker thread on new connection

    //threadLaunched = true;

    bool dispacterLocal_ThreadLauched = false;
    unsigned int threadLocalID = 1;

    int head;
    std::string task;

    int sockfd, client_sock;
    struct addrinfo hints;
    struct addrinfo* servinfo;
    struct addrinfo* p;
    struct sockaddr_storage clt_addr;
    socklen_t sin_size;
    char addr[INET6_ADDRSTRLEN];
    int port;
    int yes = 1;
    int rc;
    int i = maxNumConnections;

    char* bufTesting = new char[MAX_BUFFER_SIZE];

    std::vector<std::thread> threads;

    //std::vector<int> clientSocketsList;
    //clientSocketsList.reserve(maxNumConnections);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rc = getaddrinfo(this->serverAddress.c_str(), this->serverPort.c_str(), &hints, &servinfo)) != 0)
    {
        std::cerr << "Could not get address information!" << std::endl;
        return;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            std::cerr << "Could not create socket!" << std::endl;
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            std::cerr << "Could not configure socket!" << std::endl;
            return;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            std::cerr << "Could not bind socket!" << std::endl;
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)
    {
        std::cerr << "Server failed to bind!\np is NULL" << std::endl;
        return;
    }

    if (listen(sockfd, this->maxNumConnections) == -1)
    {
        std::cerr << "Server failed to listen!\nMaximum number of connections before even listening" << std::endl;
        return;
    }

    Utilites::Mutex_Print(-1, "Server started and waiting for connections!");
    Utilites::Mutex_Print(-1, "", false);

    threadLaunched = true;

    //std::cout << "> ";
    //Utilites::Mutex_Print(-1, "> ", false);

    while (!quitCommandExecuted)
    {
        sin_size = sizeof(clt_addr);
        client_sock = accept(sockfd, (struct sockaddr*)&clt_addr, &sin_size);

        if (client_sock == -1)
        {
            std::cerr << "Server failed to accept client!\nclient_sock value is -1" << std::endl;
            continue;
        }

        if (((struct sockaddr*)&clt_addr)->sa_family == AF_INET)
        {
            struct sockaddr_in* s = (struct sockaddr_in*)&clt_addr;
            port = ntohs(s->sin_port);

            inet_ntop(clt_addr.ss_family, &(((struct sockaddr_in*)&clt_addr)->sin_addr), addr, sizeof(addr));
        }
        else
        {
            struct sockaddr_in6* s = (struct sockaddr_in6*)&clt_addr;
            port = ntohs(s->sin6_port);
            inet_ntop(clt_addr.ss_family, &(((struct sockaddr_in6*)&clt_addr)->sin6_addr), addr, sizeof(addr));
        }

        if (!quitCommandExecuted)
        {
            Utilites::Mutex_Print(-1, "Server got connection from ", false);
            std::cout << addr << "\n";
            //Utilites::Mutex_Print(-1, "!\n", false);
            std::cout << "> ";

            if (threadLocalID < 10)
            {
                sprintf(bufTesting, "%s%d", "ClientName: client0", threadLocalID);
            }
            else
            {
                sprintf(bufTesting, "%s%d", "ClientName: client", threadLocalID);
            }

            if (send(client_sock, bufTesting, strlen(bufTesting) + 1, 0) == -1)
            {
                std::cerr << "Error sending ClientName Message!" << std::endl;

                delete[] bufTesting;

                close(client_sock);
                break;
            }

            clientList[client_sock] = ClientData(addr, port, threadLocalID);
            //clientSocketsList.emplace_back(client_sock);

            threads.emplace_back(std::thread(&ServerSideEngine::runSubWorker, this, ++threadLocalID, std::ref(dispacterLocal_ThreadLauched), client_sock));

            while (!dispacterLocal_ThreadLauched);

            dispacterLocal_ThreadLauched = !dispacterLocal_ThreadLauched;

            i--;
        }
    }

    SendQuitEventAllClients();

    for (int i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }

    //close(sockfd);

    delete[] bufTesting;
}

void ServerSideEngine::runSubWorker(const unsigned int thread_id, bool& threadLaunched, const int sock)
{
    threadLaunched = true;

    char* bufTesting = new char[MAX_BUFFER_SIZE];
    //std::string buf;
    int numBytes;

    //buf.reserve(MAX_BUFFER_SIZE - 1);
    //buf.clear();

    std::istringstream iss;
    std::string token1, token2, token3;
    //const std::string local_client_ID = std::to_string(thread_id) + ":";

    //bool quit = false;
    //threadsBoolStorage.emplace_back(&quit);

    //while (!quit)
    while (true)
    {
        memset(bufTesting, 0, MAX_BUFFER_SIZE);
        if ((numBytes = recv(sock, bufTesting, MAX_BUFFER_SIZE - 1, 0)) == -1)
        {
            // Need to rewrite this for server application.
            std::cerr << "Error receiving result, after sending indexing data!" << std::endl;
            std::cerr << "Error: numBytes: " << numBytes << "\n";
            std::cerr << "Aborting the connection!" << std::endl;

            delete[] bufTesting;

            close(sock);
            auto& clientData = this->clientList.find(sock)->second;
            //clientData.isConnected = false;

            this->clientList.erase(sock);
            //--ServerSideEngine::ClientData::threadLocalID;
            return;
        }

        if (numBytes > 0)
        {
            //std::cout << "Data Transfer from Client Successful\n";
            //buf = bufTesting;

            //if (buf.size() >= 4 && buf.substr(0, 4) == "quit")
            //if (strlen(bufTesting) >= 4 && (strncmp(bufTesting, "quit", 4) == 0))
            if (numBytes == 4 && (strncmp(bufTesting, "quit", 4) == 0))
            {
                // Need to rewrite this for server application.
                std::cout << "Received quit from " << this->clientList.find(sock)->second.clientAddress << " client" << std::endl;

                delete[] bufTesting;

                close(sock);
                auto& clientData = this->clientList.find(sock)->second;
                //clientData.isConnected = false;
                
                this->clientList.erase(sock);
                //--ServerSideEngine::ClientData::threadLocalID;
                return;
            }

            //if (buf.size() >= 6 && buf.substr(0, 6) == "Index:")
            //if (strlen(bufTesting) >= 6 && (strncmp(bufTesting, "Index:", 6) == 0))
            if (numBytes >= 6 && (strncmp(bufTesting, "Index:", 6) == 0))
            {
                iss.str("");
                iss.clear();
                //iss.str(buf);
                iss.str(bufTesting);

                token1.clear();
                token2.clear();
                token3.clear();

                iss >> token1;

                //std::cout << token1 << "\n";
                assert(token1 == "Index:");

                //std::cout << "Successful I/S commands received: " << buf.size() << std::endl;
                //std::cout << "Successful I/S commands received: " << strlen(bufTesting) << std::endl;

                if (send(sock, "Success", 7, 0) == -1)
                {
                    std::cerr << "Error sending index data successful Message!" << std::endl;
                    std::cerr << "Aborting the connection!" << std::endl;

                    delete[] bufTesting;

                    close(sock);
                    auto& clientData = this->clientList.find(sock)->second;
                    //clientData.isConnected = false;
                    
                    this->clientList.erase(sock);
                    //--ServerSideEngine::ClientData::threadLocalID;
                    return;
                }

                //std::cout << "Sent index data successful Message!" << std::endl;

                //int i = 0;

                while (iss >> token1 >> token2 >> token3)
                {
                    //token2 = local_client_ID + token2;
                    this->store->insertIndex(token1, token2, std::stoi(token3));

                    /*if (++i < 2)
                    {
                        std::cout << token1 << "\n" << token2 << "\n" << token3 << "\n" << "\n\n";
                    }*/
                }

                //std::cout << "Done with the insertion in the index store.\n";

                //buf.clear();
                memset(bufTesting, 0, MAX_BUFFER_SIZE);
                continue;
            }

            if (numBytes >= 6 && (strncmp(bufTesting, "Search:", 6) == 0))
            {
                iss.str("");
                iss.clear();
                //iss.str(buf);
                iss.str(bufTesting);

                token1.clear();
                token2.clear();
                token3.clear();

                iss >> token1;

                //std::cout << token1 << "\n";
                assert(token1 == "Search:");

                //std::cout << "Successful I/S commands received: " << buf.size() << std::endl;
                //std::cout << "Successful I/S commands received: " << strlen(bufTesting) << std::endl;

                PerformWordsSearch(bufTesting, numBytes, sock);

                //buf.clear();
                memset(bufTesting, 0, MAX_BUFFER_SIZE);
                continue;
            }

            //if (buf.size() >= 26 && buf.substr(0, 27) == "Client-ShutDown-Successful")
            //if (strlen(bufTesting) >= 26 && (strncmp(bufTesting, "Client-ShutDown-Successful", 27) == 0))
            if (numBytes == 26 && (strncmp(bufTesting, "Client-ShutDown-Successful", 26) == 0))
            {
                std::cout << "Client Sent Successful shutdown message\n";
                break;
            }

            if (numBytes == 30 && (strncmp(bufTesting, "Finished indexing client side.", 30) == 0))
            {
                std::cout << "Finished indexing server side.\n";

                //buf.clear();
                memset(bufTesting, 0, MAX_BUFFER_SIZE);
                continue;
            }

            std::cout << "Successful receive numBytes, but no logic handled the received bytes: " << numBytes << " " << bufTesting << "\n";

            //buf.clear();
            memset(bufTesting, 0, MAX_BUFFER_SIZE);
        }

        //buf.clear();
        memset(bufTesting, 0, MAX_BUFFER_SIZE);
    }

    delete[] bufTesting;
}

void ServerSideEngine::PerformWordsSearch(const char* _input, const int numBytes, const int sock)
{
    //std::cout << "Testing input: " << _input << std::endl;

    //const std::string command = _input;
    std::unordered_map<std::string, unsigned int> result;
    std::unordered_map<std::string, unsigned int> temporary;
    
    std::istringstream iss;
    std::string word;

    bool found = false;

    iss.str("");
    iss.clear();
    iss.str(_input);

    word.clear();

    iss >> word;

    //std::cout << word << std::endl;

    while (iss >> word)
    {
        /*
        Look Up in Server here
        */

        //std::cout << word << std::endl;

        found = this->store->lookupIndex(word, temporary);

        if (found)
        {
            //std::cout << "Found something on search\n";
            /*
            Combing every result from the server below.
            */

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

    std::string searchResults;

    if (result.size() == 0)
    {
        searchResults += "Success-Search: Empty";
    }
    else
    {
        searchResults += "Success-Search: ";

        for (const auto& pair : result)
        {
            searchResults += std::to_string(pair.second) + " " + pair.first + " ";
            //results.list[pair.second] = pair.first;
        }

        //std::cout << "Search Results: " << searchResults << std::endl;
    }

    if (searchResults.size() > MAX_BUFFER_SIZE - 1)
    {
        std::cerr << "Spliting is not Implemented.\n";
        searchResults.clear();
        searchResults += "Success-Search: Overflow";
    }

    char* buf = new char[searchResults.size() + 1];
    memset(buf, 0, searchResults.size() + 1);
    memcpy(buf, searchResults.c_str(), searchResults.size());

    if (send(sock, buf, searchResults.size(), 0) == -1)
    {
        std::cerr << "Error sending search data successful Message!" << std::endl;
        std::cerr << "Aborting the connection!" << std::endl;

        delete[] buf;

        close(sock);
        auto& clientData = this->clientList.find(sock)->second;
        //clientData.isConnected = false;

        this->clientList.erase(sock);
        //--ServerSideEngine::ClientData::threadLocalID;
        return;
    }

    std::cout << "Search complete!" << std::endl;

    delete[] buf;
}

void ServerSideEngine::SendQuitEventAllClients()
{
    // showdown the threads.
    /*for (const auto threadQuit : threadsBoolStorage)
    {
        *threadQuit = true;
    }*/

    // send quit to all servers.
    for (auto& value : clientList)
    {
        if (send(value.first, "Server-ShutDown", 15, 0) == -1)
        {
            std::cerr << "Error sending Server-ShutDown Message!" << std::endl;

            //close(value.first);
            //this->clientList.erase(value.first);
        }

        close(value.first);
    }

    // close all the connections.
    //for (auto value : clientList)
    //{
    //    close(value.first);
    //}
}

void ServerSideEngine::OnServerQuitEvent()
{
    int sockfd;
    struct addrinfo hints;
    struct addrinfo* servinfo;
    struct addrinfo* p;
    char addr[INET6_ADDRSTRLEN];
    int rc;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rc = getaddrinfo(this->serverAddress.c_str(), this->serverPort.c_str(), &hints, &servinfo)) != 0)
    {
        std::cerr << "Could not get address information!" << std::endl;
        return;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            std::cerr << "Could not create socket!" << std::endl;
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            std::cerr << "Could not connect to server!" << std::endl;
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

    freeaddrinfo(servinfo);

    clientList[sockfd] = ClientData(addr, atoi(this->serverPort.c_str()), -1);
    //close(sockfd);
}

void ServerSideEngine::shutdown()
{
    quitCommandExecuted = true;

    OnServerQuitEvent();
    
    dispatcherThread.join();

    /*for (int i = 0; i < this->threads.size(); ++i)
    {
        this->threads[i].join();
    }*/

    std::cout << "Shutting down the server" << std::endl;
}