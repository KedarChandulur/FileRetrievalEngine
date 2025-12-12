cmake_minimum_required(VERSION 3.22)

project(file-retrieval-engine
        VERSION 1.0
        DESCRIPTION "C++ gRPC File Retrieval Engine"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig)
pkg_search_module(GRPC REQUIRED grpc++)

find_package(Protobuf REQUIRED)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(FileRetrieval proto/file_retrieval.proto)
target_link_libraries(FileRetrieval PUBLIC ${GRPC_LIBRARIES} ${Protobuf_LIBRARIES})
protobuf_generate(TARGET FileRetrieval)
protobuf_generate(
    TARGET FileRetrieval
    LANGUAGE grpc
    PLUGIN protoc-gen-grpc=/usr/bin/grpc_cpp_plugin
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
)

add_executable(file-retrieval-server
               src/file-retrieval-server.cpp
               src/ServerAppInterface.cpp
               src/ServerSideEngine.cpp
               src/IndexStore.cpp
               src/FileRetrievalImpl.cpp)
target_include_directories(file-retrieval-server PUBLIC include)
target_link_libraries(file-retrieval-server FileRetrieval)

add_executable(file-retrieval-client
               src/file-retrieval-client.cpp
               src/ClientAppInterface.cpp
               src/ClientSideEngine.cpp)
target_include_directories(file-retrieval-client PUBLIC include)
target_link_libraries(file-retrieval-client FileRetrieval)