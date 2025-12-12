#include "FileRetrievalImpl.hpp"

grpc::Status FileRetrievalImpl::Index(grpc::ServerContext* context, const IndexRequest* request, IndexReply* reply)
{
    // TO-DO implement the index functionality
    // Call index on the server
    return grpc::Status::OK;
}

grpc::Status FileRetrievalImpl::Search(grpc::ServerContext* context, const SearchRequest* request, SearchReply* reply)
{
    // TO-DO implement the search functionality
    // Call search on the server
    return grpc::Status::OK;
}