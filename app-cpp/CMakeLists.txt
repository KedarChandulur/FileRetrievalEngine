cmake_minimum_required(VERSION 3.22)

project(file-retrieval-engine
        VERSION 1.0
        DESCRIPTION "C++ ZeroMQ File Retrieval Engine"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(CPPZMQ_BUILD_TESTS "Whether or not to build the tests" OFF)
add_subdirectory(cppzmq EXCLUDE_FROM_ALL)

add_executable(file-retrieval-server
               src/file-retrieval-server.cpp
               src/ServerAppInterface.cpp
               src/ServerSideEngine.cpp
               src/IndexStore.cpp)
target_link_libraries(file-retrieval-server cppzmq)
target_include_directories(file-retrieval-server PUBLIC include)

add_executable(file-retrieval-client
               src/file-retrieval-client.cpp
               src/ClientAppInterface.cpp
               src/ClientSideEngine.cpp)
target_link_libraries(file-retrieval-client cppzmq)
target_include_directories(file-retrieval-client PUBLIC include)