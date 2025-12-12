package csc435.app;

public class ServerSideEngine {
    private IndexStore store;

    public ServerSideEngine(IndexStore store) {
        this.store = store;
        // TO-DO implement constructor
    }

    public void initialize() {
        // TO-DO create one dispatcher thread
    }

    public void spawnWorker() {
        // TO-DO create a new worker thread
    }

    public void shutdown() {
        // TO-DO join the dispatcher and worker threads
    }

    public void list() {
        // TO-DO get the connected clients information and return the information
    }
}
