#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <chrono>

void CleanFile(const std::filesystem::directory_entry& entry, std::string& output, std::size_t& dataset_size)
{
    std::ifstream file(entry.path(), std::ios::binary);

    //std::cout << entry.path() << '\n';

    if (!file.is_open())
    {
        std::cerr << "Error opening file." << std::endl;

        assert(false);
    }

    file.seekg(0, std::ios::end);
    dataset_size += file.tellg();
    file.seekg(0, std::ios::beg);

    output.clear();

    std::stringstream buffer;
    buffer << file.rdbuf();
    output = buffer.str();

    file.close();

    auto isUnwantedChar = [&](const char& c) {

        if ((int)c < 0)
        {
            return true;
        }

        // Condition 4: Remove any repeating sequence of delimiters.
        // Replace it with the last delimiter in the sequence.
        if (c == ' ' || c == '\t' || c == '\n') {
            size_t index = &c - &output[0];

            if (index + 1 >= output.length()) {
                return false;
            }

            if (output[index + 1] == ' ' || output[index + 1] == '\t' || output[index + 1] == '\n' || output[index + 1] == '\r') {
                return true;
            }

            // Condition 3: Remove character if it's '\r' only if its next element is not a '\n'.
            if (c == '\r') {
                return true;
            }

            return false;
        }      

        return !(std::isalnum(c));

        //// Condition 1: Don't remove if it's alpha-numeric character.
        //if (std::isalnum(c)) {
        //    return false;
        //}

        //// Condition 2: Any separator must be eliminated.
        //return true; // Default: remove the character if it doesn't meet any condition.
        };

    output.erase(std::remove_if(output.begin(), output.end(), isUnwantedChar), output.end());
    output.erase(std::remove_if(output.begin(), output.end(), isUnwantedChar), output.end());

}

inline char GetSlash()
{
    // Need back slash here for Windows.
    //return '\\';

    // Need forward slash here for linux.
    return '/';
}

void clean_dataset(const std::string input_dir, const std::string output_dir, std::size_t& dataset_size, double& execution_time)
{
    // TO-DO implement clean dataset logic
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

    std::filesystem::path startPath(input_dir);

    //std::cout << startPath << '\n';

    output.clear();

    output = startPath.string();

    size_t lastBackslashPos = output.find_last_of(GetSlash());

    // CleanedData base folder creation
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

            output = "Cleaned" + temp;
            currentDatasetDirectory.clear();
            currentDatasetDirectory = output = modified_output_dir + output;

            temp = temp.substr(0, temp.length() - 1);
            std::cout << "Started cleaning: " << temp << "\n";
        }
        else
        {
            output = "Cleaned" + temp;
            currentDatasetDirectory.clear();

            //std::cout << "Final Result: " << output << std::endl << std::endl;
            std::cout << "Started cleaning: " << temp << "\n";

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
                    CleanFile(dir_entry, output, dataset_size);

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
    std::size_t dataset_size = 0;
    double execution_time = 0.0;

    if (argc != 3) 
    {
        std::cerr << "improper number of arguments" << std::endl;
        return 1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    clean_dataset(std::string(argv[1]), std::string(argv[2]), dataset_size, execution_time);

    auto end_time = std::chrono::high_resolution_clock::now();
    execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        
    double stringSizeMiB = static_cast<double>(dataset_size) / (1024 * 1024);

    std::cout << "Finished cleaning " << stringSizeMiB << " MiB of data";
    std::cout << " in " << execution_time << " miliseconds" << std::endl;

    return 0;
}