package csc435.app;

import io.grpc.stub.StreamObserver;

public class ServerSideEngine {
    private IndexStore store;

    public ServerSideEngine(IndexStore store) {
        this.store = store;
        // TO-DO implement constructor
    }

    public void initialize() {
        // TO-DO create and register the gRPC service
    }

    public void index() {
        // TO-DO implement the server-side indexing operation (only what the server would process when indexing)
    }

    public void search() {
        // TO-DO implement the server-side search operation (only what the server would process when searching)
    }

    public void shutdown() {
        // TO-DO implement graceful shutdown of the gRPC service
    }

    private static class FileRetrievalService extends FileRetrievalGrpc.FileRetrievalImplBase {
        
        @Override
        public void index(IndexRequest request, StreamObserver<IndexReply> respObserver) {
            respObserver.onNext(doIndex(request));
            respObserver.onCompleted();
        }

        @Override
        public void search(SearchRequest request, StreamObserver<SearchReply> respObserver) {
            respObserver.onNext(doSearch(request));
            respObserver.onCompleted();
        }

        private IndexReply doIndex(IndexRequest request) {
            // TO-DO implement the index functionality
            // Call index on the server
            return IndexReply.newBuilder().build();
        }

        private SearchReply doSearch(SearchRequest request) {
            // TO-DO implement the search functionality
            // Call search on the server
            return SearchReply.newBuilder().build();
        }
    }
}
