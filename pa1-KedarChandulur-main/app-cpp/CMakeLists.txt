cmake_minimum_required(VERSION 3.22)

project(app-cpp
        VERSION 1.0
        DESCRIPTION "Clean Count Sort Applications"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)



add_executable(clean_dataset
               src/clean_dataset.cpp)
target_include_directories(clean_dataset PUBLIC include)

add_executable(count_words
               src/count_words.cpp)
target_include_directories(count_words PUBLIC include)

add_executable(sort_words
               src/sort_words.cpp)
target_include_directories(sort_words PUBLIC include)