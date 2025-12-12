package csc435.app;

public class Worker implements Runnable {
    private IndexStore store;

    public Worker(IndexStore store) {
        this.store = store;
        // TO-DO implement constructor
    }
    
    @Override
    public void run() {
        // TO-DO receive index and search commands from the client until the client disconnects
    }
}
