#include "IndexStore.hpp"

#include <iostream>

void IndexStore::insertIndex(const std::string& word, const std::string& filePath, const unsigned int frequency)
{
    std::lock_guard<std::mutex> guard(this->mtx);

    const auto& it = indexMap.find(word);

    if (it != indexMap.end())
    {
        //std::cout << "old: " << filePath << it->second.list.size() << std::endl;
        
        // Update existing entry
        if (it->second.list.size() < IndexingData::MAX_SIZE || frequency > it->second.list.rbegin()->first)
        {
            it->second.list.emplace(frequency, filePath);

            if (it->second.list.size() > IndexingData::MAX_SIZE)
            {
                it->second.list.erase(--it->second.list.end());
            }
        }
    }
    else
    {
        // Create new entry
        //std::cout << "New w: " << word << " fq: " << frequency << " fp: " << filePath << std::endl;
        indexMap.emplace(word, IndexingData(frequency, filePath));
    }
}

//bool IndexStore::lookupIndex(const std::string& word, IndexingData& resultOut) 
bool IndexStore::lookupIndex(const std::string& word, std::unordered_map<std::string, unsigned int>& resultOut)
{
    auto it = indexMap.find(word);

    if (it != indexMap.end())
    {
        //resultOut.list = it->second.list;

        for (const auto& pair : it->second.list)
        {
            resultOut[pair.second] = pair.first;
        }

        return true;
    }

    return false;
}