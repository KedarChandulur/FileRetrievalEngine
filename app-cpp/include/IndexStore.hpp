#ifndef INDEX_STORE_H
#define INDEX_STORE_H

#include <unordered_map>
#include <mutex>
#include <map>
#include <unordered_map>

class IndexStore
{
public:
    struct IndexingData
    {
        // Use map sorted by frequency in descending order
        std::map<unsigned int, std::string, std::greater<unsigned int>> list;

        static const unsigned short MAX_SIZE = 10; // Limit to 10 entries

        IndexingData() = default;
        ~IndexingData() = default;

        IndexingData(unsigned int frequency, const std::string& filePath)
        {
            if (list.size() < IndexingData::MAX_SIZE)
            {
                list.emplace(frequency, filePath);
            }
            else if (frequency > list.rbegin()->first)
            {
                // Remove the least frequent entry if new frequency is higher
                list.erase(--list.end());
                list.emplace(frequency, filePath);
            }
        }
    };

public:
    // constructor
    IndexStore() = default;

    // default virtual destructor
    virtual ~IndexStore() = default;

    // Insert new data
    void insertIndex(const std::string& word, const std::string& filePath, const unsigned int frequency);

    // Lookup by word
    bool lookupIndex(const std::string& word, std::unordered_map<std::string, unsigned int>& resultOut);

private:
    std::unordered_map<std::string, IndexingData> indexMap;
    std::mutex mtx;
};

#endif