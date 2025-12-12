package csc435.app;


public class Dispatcher implements Runnable {
    private ServerSideEngine engine;

    public Dispatcher(ServerSideEngine engine) {
        this.engine = engine;
        // TO-DO implement constructor
    }
    
    @Override
    public void run() {
        // TO-DO create a TCP/IP socket and listen for new connections
        // When new connection comes through create a new Worker thread for the new connection
    }
}
