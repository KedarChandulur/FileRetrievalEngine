#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <map>
#include <vector>
#include <iterator>

void OutputToFile(std::map<int, std::string, std::greater<int>>& wordMap, std::string& output)
{
    output.clear();

    for (const auto& pair : wordMap)
    {
        output += pair.second + " " + std::to_string(pair.first) + "\n";
    }
}

void ProcessWords(std::vector<std::string>& words, std::stringstream& buffer, std::map<int, std::string, std::greater<int>>& wordMap, std::size_t& num_words)
{
    words.clear();
    words = { std::istream_iterator<std::string>{buffer}, std::istream_iterator<std::string>() };

    num_words = num_words + (words.size() / 2);

    wordMap.clear();
    bool IsEven = true;
    std::string temporaryString = "";
    unsigned int temporaryInteger = 0;

    for (const std::string& word : words)
    {
        if (IsEven)
        {
            //std::cout << word << " ";
            temporaryString = word;
        }
        else
        {
            //std::cout << word << "\n";
            temporaryInteger = std::stoi(word);
            wordMap.insert(std::make_pair(temporaryInteger, temporaryString));
        }

        IsEven = !IsEven;
    }
}

void LoadFile(const std::filesystem::directory_entry& entry, std::ifstream& file, std::stringstream& buffer)
{
    if (file.is_open())
    {
        file.close();
    }

    file.clear();

    file.open(entry.path(), std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error opening file." << std::endl;

        assert(false);
    }

    buffer.clear();
    buffer << file.rdbuf();

    file.close();
}

inline char GetSlash()
{
    // Need back slash here for Windows.
    //return '\\';

    // Need forward slash here for linux.
    return '/';
}

void sort_words(std::string input_dir, std::string output_dir, std::size_t& num_words, double& execution_time)
{
    // TO-DO implement count dataset logic
    std::string modified_output_dir(output_dir);

    size_t lb_of = modified_output_dir.find_last_of(GetSlash());

    std::string t = modified_output_dir.substr(lb_of + 1);

    if (t != "")
    {
        modified_output_dir += GetSlash();
    }
    /*else
    {
        std::cout << "Last brace set" << std::endl;
    }*/

    std::string currentDatasetDirectory = "";
    std::string currentDatasetFolder = "";
    std::string output;

    std::ifstream file;
    std::stringstream buffer;

    std::vector<std::string> words;
    std::map<int, std::string, std::greater<int>> wordMap;

    std::filesystem::path startPath(input_dir);

    //std::cout << startPath << '\n';

    output.clear();

    output = startPath.string();

    size_t lastBackslashPos = output.find_last_of(GetSlash());

    // CountedData base folder creation
    // Check if backslash is found
    if (lastBackslashPos != std::string::npos)
    {
        std::string temp;
        // Copy the string after the last backslash
        temp = output.substr(lastBackslashPos + 1);

        if (temp == "")
        {
            size_t secondlastBackslashPos = output.find_last_of(GetSlash(), lastBackslashPos - 1);

            temp = output.substr(secondlastBackslashPos + 1);

            assert(temp.find("Dataset") != std::string::npos);

            temp.erase(temp.begin(), temp.begin() + 7);
            output = "Sorted" + temp;

            currentDatasetDirectory.clear();
            currentDatasetDirectory = output = modified_output_dir + output;

            temp = temp.substr(0, temp.length() - 1);
            std::cout << "Started sorting: Counted" << temp << "\n";
        }
        else
        {
            temp.erase(temp.begin(), temp.begin() + 7);
            output = "Sorted" + temp;

            currentDatasetDirectory.clear();

            std::cout << "Started sorting: Counted" << temp << "\n";

            currentDatasetDirectory = output = modified_output_dir + output + GetSlash();
        }

        if (!std::filesystem::exists(currentDatasetDirectory))
        {
            //std::cout << "Creating Dir: " << currentDatasetDirectory << std::endl << std::endl;

            if (!std::filesystem::create_directory(currentDatasetDirectory))
            {
                assert(false);
            }

            //std::cout << "Directory created successfully." << std::endl;
        }
        /*else
        {
            std::cout << "Directory '" << currentDatasetDirectory << "' already exists." << std::endl;
        }*/
    }
    else
    {
        // If backslash is not found, something is wrong.
        std::cout << "Result: " << output << std::endl;
        assert(false);
    }

    // Recursion sub dir.
    if (std::filesystem::exists(startPath) && std::filesystem::is_directory(startPath))
    {
        //std::cout << "Files and folders in " << startPath << ":\n";         

        for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(startPath))
        {
            if (!dir_entry.is_directory())
            {
                //std::cout << dir_entry << '\n';

                output.clear();

                output = dir_entry.path().string();

                size_t lastBackslashPos = output.find_last_of(GetSlash());

                output = output.substr(lastBackslashPos + 1);

                // Print the result
                //std::cout << "Result: " << output << std::endl;
                std::cout << "  Processing File = " << output << "\n";

                std::string filePath = currentDatasetFolder + GetSlash() + output;

                // Create an ofstream object to write to the file
                std::ofstream outputFile(filePath);

                // Check if the file is successfully opened
                if (outputFile.is_open())
                {
                    // Write content to the file
                    LoadFile(dir_entry, file, buffer);
                    ProcessWords(words, buffer, wordMap, num_words);
                    OutputToFile(wordMap, output);

                    //dataset_size += output.size() * sizeof(char);

                    outputFile << output;

                    // Close the file
                    outputFile.close();

                    //std::cout << "Text file created successfully: " << filePath << std::endl;
                }
                else
                {
                    std::cerr << "Error opening the file: " << filePath << std::endl;
                    assert(false);
                }
            }
            else
            {
                //std::cout << dir_entry << '\n';

                output.clear();

                output = dir_entry.path().string();

                size_t lastBackslashPos = output.find_last_of(GetSlash());

                // Check if backslash is found
                if (lastBackslashPos != std::string::npos)
                {
                    // Copy the string after the last backslash
                    output = output.substr(lastBackslashPos + 1);

                    assert(output.find("folder") != std::string::npos);
                    std::cout << "\nProcessing Folder = " << output << "\n";

                    currentDatasetFolder.clear();
                    //std::cout << "Final Result: " << output << std::endl << std::endl;
                    currentDatasetFolder = output = currentDatasetDirectory + output;

                    if (!std::filesystem::exists(currentDatasetFolder))
                    {
                        //std::cout << "Creating Dir: " << currentDatasetFolder << std::endl << std::endl;

                        if (!std::filesystem::create_directory(currentDatasetFolder))
                        {
                            assert(false);
                        }

                        //std::cout << "Directory created successfully." << std::endl;
                    }
                    /*else
                    {
                        std::cout << "Directory '" << currentDatasetFolder << "' already exists." << std::endl;
                    }*/
                }
                else
                {
                    // If backslash is not found, something is wrong.
                    std::cout << "Result: " << output << std::endl;
                    assert(false);
                }
            }
        }
    }
    else
    {
        std::cerr << "Invalid directory path.\n";
        std::cerr << "Did you provide the direct file path? Only directory/folder paths allowed.\n";
        assert(false);
    }
}

int main(int argc, char** argv)
{
    std::size_t num_words = 0;
    double execution_time = 0.0;

    if (argc != 3) {
        std::cerr << "improper number of arguments" << std::endl;
        return 1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    sort_words(std::string(argv[1]), std::string(argv[2]), num_words, execution_time);

    auto end_time = std::chrono::high_resolution_clock::now();
    execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    //double stringSizeMiB = static_cast<double>(num_words) / (1024 * 1024);

    std::cout << "Finished sorting " << num_words << " words";
    std::cout << " in " << execution_time << " miliseconds" << std::endl;

    return 0;
}