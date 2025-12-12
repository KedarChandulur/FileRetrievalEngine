package csc435.app;

public class ClientSideEngine {
    // TO-DO keep track of the connection

    public ClientSideEngine() {
        
        // TO-DO implement constructor
    }

    public void indexFiles() {
        // TO-DO implement index files method
        // for each file read and count the words and send the counted words to the server
    }
    
    public void searchFiles() {
        // TO-DO implement search files method
        // for each term contact the server to retrieve the list of documents that contain the word
        // combine the results of a multi-term query
        // return top 10 results
    }

    public void openConnection() {
        // TO-DO implement connect to server
        // create a new TCP/IP socket and connect to the server
    }

    public void closeConnection() {
        // TO-DO implement disconnect from server
        // close the TCP/IP socket
    }
}
