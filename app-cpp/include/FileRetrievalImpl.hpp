#ifndef FILE_RETRIEVAL_H
#define FILE_RETRIEVAL_H

#include <grpc/grpc.h>
#include <grpcpp/server_context.h>

#include "proto/file_retrieval.grpc.pb.h"

using file_retrieval::FileRetrieval;
using file_retrieval::IndexRequest;
using file_retrieval::IndexReply;
using file_retrieval::SearchRequest;
using file_retrieval::SearchReply;

class FileRetrievalImpl final : public FileRetrieval::Service
{
    public:
        explicit FileRetrievalImpl() { }
        grpc::Status Index(grpc::ServerContext* context, const IndexRequest* request, IndexReply* reply);
        grpc::Status Search(grpc::ServerContext* context, const SearchRequest* request, SearchReply* reply);
        
};

#endif