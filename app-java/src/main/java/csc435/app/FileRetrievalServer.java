package csc435.app;

public class FileRetrievalServer
{
    public static void main( String[] args )
    {
        IndexStore store = new IndexStore();
        ServerSideEngine engine = new ServerSideEngine(store);
        ServerAppInterface appInterface = new ServerAppInterface(engine);
        
        engine.initialize();
        appInterface.readCommands();
    }
}
