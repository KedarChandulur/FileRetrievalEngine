#include <mutex>
#include <iostream>

#include "Utilities.hpp"

//std::mutex Utilites::cout_mutex;

void Utilites::Mutex_Print(const int id, const char* message, bool printEndLine)
{
    static std::mutex cout_mutex;
    std::lock_guard<std::mutex> guard(cout_mutex);

    if (id < 1)
    {
        std::cout << message;

        if (printEndLine)
        {
            std::cout << "\n";
        }
    }
    else
    {
        std::cout << "Executing from the thread id: " << id << "\n\t";
        std::cout << "Message: " << message << "\n";
    }
}