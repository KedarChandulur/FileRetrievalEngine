#include "AppInterface.hpp"

#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include <random>

AppInterface::AppInterface(std::shared_ptr<ProcessingEngine> engine) : engine(engine) 
{
}

void AppInterface::Test100Words(std::vector<std::string>& input, std::vector<std::string>& words, const std::vector<int>& random_indices, IndexStore::IndexingData& result)
{
    // Now you have 100 random indices in `random_indices`, and you can use them to access words in the `words` vector:
    for (const int index : random_indices)
    {
        input.emplace_back(std::move(words[index]));

        this->engine->searchFiles(input, result);

        input.clear();
    }
}

void AppInterface::LoadFile(const std::string& filepath, std::vector<std::string>& words, std::vector<int>& random_indices)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string word;
    while (file >> word) 
    {
        words.emplace_back(word);
    }

    std::random_device rd;
    std::mt19937 gen(rd()); // Seed the random number generator
    std::uniform_int_distribution<> dist(0, words.size() - 1); // Generate numbers within word count range

    for (int i = 0; i < 100; ++i) 
    {
        int random_index = dist(gen);
        random_indices.emplace_back(random_index);
    }
}

void AppInterface::readCommands() 
{
    // TO-DO implement the read commands method
    std::string command;
    std::string temp;
    std::vector<std::string> wordsInput;
    unsigned char ch;
    //std::vector<IndexStore::IndexingData> results;
    IndexStore::IndexingData result;
    
    while (true) {
        std::cout << "> ";
        
        // read from command line
        getline(std::cin, command);

        // if the command is quit, terminate the program       
        if (command == "quit") 
        {
            engine->stopWorkers();
            break;
        }
        
        // if the command begins with index, index the files from the specified directory
        if (command.size() >= 5 && command.substr(0, 5) == "index") 
        {
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
            if (command.size() == 6 || command.size() == 7)
            {
                std::cout << "Please insert word(s), if you are using search command\nSyntax of the command: search <word>\nOptionally you can also use multiple words in the search\nSyntax of the command: search <word> AND <word>\n";
                continue;
            }

            assert(command[6] == ' ');

            wordsInput.clear();
            temp.clear();

            for (unsigned int i = 7; i < command.length(); ++i)
            {
                ch = command[i];

                if (ch == ' ')
                {
                    wordsInput.emplace_back(temp);
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
            }

            //results.clear();

            auto start_time = std::chrono::high_resolution_clock::now();

            //engine->searchFiles(wordsInput, results);
            engine->searchFiles(wordsInput, result);

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
                    std::cout << "* " << it.second << " " << it.first << std::endl;
                }
            }
            else
            {
                std::cout << "Search results (top 10):" << std::endl;

                for (const auto& it : result.list)
                {
                    std::cout << "* " << it.second << " " << it.first << std::endl;
                }
            }

            continue;
        }

        // Search latency test.
        if (command.size() >= 4 && command.substr(0, 4) == "s100")
        {
            if (command.size() == 4 || command.size() == 5)
            {
                std::cout << "Please insert file path, if you are using index command\nSyntax of the command: index <filepath>\n";
                continue;
            }

            assert(command[4] == ' ');

            // path stored in temp.
            temp = command.substr(5);

            IndexStore::IndexingData result;
            std::vector<std::string> input;
            input.reserve(1);
            std::vector<std::string> words;
            std::vector<int> random_indices;
            random_indices.reserve(100);

            this->LoadFile(temp, words, random_indices);

            auto start_time = std::chrono::high_resolution_clock::now();

            this->Test100Words(input, words, random_indices, result);

            auto end_time = std::chrono::high_resolution_clock::now();

            double execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
            std::cout << "Completed s100 in " << execution_time << " microseconds" << std::endl;

            continue;
        }

        std::cout << "unrecognized command!, search in lower case letters only" << std::endl;
    }
}