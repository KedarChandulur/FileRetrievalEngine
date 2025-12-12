#ifndef APP_INTERFACE_H
#define APP_INTERFACE_H

#include <memory>

#include "ProcessingEngine.hpp"

class AppInterface
{
    std::shared_ptr<ProcessingEngine> engine;

    public:
        // constructor
        AppInterface(std::shared_ptr<ProcessingEngine> engine);

        // default virtual destructor
        virtual ~AppInterface() = default;

        void readCommands();
        void LoadFile(const std::string& filepath, std::vector<std::string>& words, std::vector<int>& random_indices);
        void Test100Words(std::vector<std::string>& input, std::vector<std::string>& words, const std::vector<int>& random_indices, IndexStore::IndexingData& result);
};

#endif