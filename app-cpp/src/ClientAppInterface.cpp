#include <iostream>
#include <string>
#include <cassert>

#include "Utilities.hpp"
#include "ClientAppInterface.hpp"

ClientAppInterface::ClientAppInterface(std::shared_ptr<ClientSideEngine> engine)
    : engine(engine)
{
}

void ClientAppInterface::readCommands() 
{
    // TO-DO implement the read commands method
    std::string command;
    std::string temp;
    std::vector<std::string> wordsInput;
    std::string              wordsInputString;
    unsigned char ch;
    //std::vector<IndexStore::IndexingData> results;
    IndexStore::IndexingData result;

    std::string tempClientAddress = "";
    std::string tempClientPort = "";

    while (true) 
    {
        Utilites::Mutex_Print(-1, "> ", false);
        
        // read from command line
        //getline(std::cin, command);

        if (!std::getline(std::cin, command))
        {
            // Handle EOF
            std::cout << "Input terminated. Quitting...\n";
            engine->closeConnection();
            break;
        }

        // if the command is quit, terminate the program       
        if (command == "quit")
        {
            // close the connection with the server
            engine->closeConnection();
            break;
        }

        // if the command begins with connect, connect to the given server
        if (command.size() >= 7 && command.substr(0, 7) == "connect")
        {
            // TO-DO call the client side engine method to connect to the server

            size_t len = command.find_last_of(' ');
            tempClientPort = command.substr(len + 1);
            
            size_t len2 = command.find_last_of(' ', len - 1);
            tempClientAddress = command.substr(len2 + 1, len - len2 - 1);

            engine->openConnection(tempClientAddress, tempClientPort);
            continue;
        }
        
        // if the command begins with index, index the files from the specified directory
        if (command.size() >= 5 && command.substr(0, 5) == "index")
        {
            // TO-DO call the client side engine method to index the documents from a folder
            if (command.size() == 5 || command.size() == 6)
            {
                std::cout << "Please insert file path, if you are using index command\nSyntax of the command: index <filepath>\n";
                continue;
            }

            assert(command[5] == ' ');

            // path stored in temp.
            temp = command.substr(6);

            auto start_time = std::chrono::high_resolution_clock::now();

            engine->indexFiles(temp);

            auto end_time = std::chrono::high_resolution_clock::now();

            double execution_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
            std::cout << "Completed indexing in " << execution_time << " seconds" << std::endl;

            continue;
        }

        // if the command begins with search, search for files that matches the query
        if (command.size() >= 6 && command.substr(0, 6) == "search")
        {
            // TO-DO call the client side engine method to search for documents given a complex query
            if (command.size() == 6 || command.size() == 7)
            {
                std::cout << "Please insert word(s), if you are using search command\nSyntax of the command: search <word>\nOptionally you can also use multiple words in the search\nSyntax of the command: search <word> AND <word>\n";
                continue;
            }

            if (command[6] != ' ')
            {
                std::cout << "Did you forgot space after the search keyword?";
                continue;
            }

            wordsInput.clear();
            wordsInputString.clear();
            wordsInputString += "Search: ";
            temp.clear();

            for (unsigned int i = 7; i < command.length(); ++i)
            {
                ch = command[i];

                if (ch == ' ')
                {
                    wordsInput.emplace_back(temp);
                    wordsInputString += temp + " ";
                    temp.clear();
                    continue;
                }

                if (ch == 'A')
                {
                    assert(i + 4 < command.length());

                    if (i + 4 < command.length() && command[i + 1] == 'N' && command[i + 2] == 'D')
                    {
                        i += 4;

                        temp += command[i];
                    }
                }
                else
                {
                    temp += ch;
                }
            }

            if (!temp.empty())
            {
                wordsInput.emplace_back(temp);
                wordsInputString += temp + " ";
            }

            //results.clear();

            auto start_time = std::chrono::high_resolution_clock::now();

            engine->searchFiles(wordsInputString, result);
            //break;


            //engine->searchFiles(wordsInput, results);
            //engine->searchFiles(wordsInput, result);

            auto end_time = std::chrono::high_resolution_clock::now();

            double execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
            std::cout << "Search completed in " << execution_time << " microseconds" << std::endl;

            if (result.list.size() == 0)
            {
                if (wordsInput.size() == 1)
                {
                    std::cout << "No results found for the keyword: " << wordsInput[0];
                }
                else
                {
                    std::cout << "No matching results found for the provided keywords" << std::endl;
                }

                std::cout << std::endl << "Try a different word." << std::endl;
            }
            else if (result.list.size() < 10)
            {
                std::cout << "Only " << result.list.size() << " matching results found:" << std::endl;

                for (const auto& it : result.list)
                {
                    std::cout << "* client" << it.second << " " << it.first << std::endl;
                }
            }
            else
            {
                std::cout << "Search results (top 10):" << std::endl;

                for (const auto& it : result.list)
                {
                    std::cout << "* client" << it.second << " " << it.first << std::endl;
                }
            }

            continue;
        }

        std::cout << "unrecognized command!" << std::endl;
    }
}