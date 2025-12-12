package csc435.app;

import java.lang.System;
import java.util.Scanner;

public class ClientAppInterface {
    private ClientSideEngine engine;

    public ClientAppInterface(ClientSideEngine engine) {
        this.engine = engine;

        // TO-DO implement constructor
        // keep track of the connection with the client
    }

    public void readCommands() {
        // TO-DO implement the read commands method
        Scanner sc = new Scanner(System.in);
        String command;
        
        while (true) {
            System.out.print("> ");
            
            // read from command line
            command = sc.nextLine();

            // if the command is quit, terminate the program       
            if (command.compareTo("quit") == 0) {
                break;
            }
            
            // if the command begins with index, index the files from the specified directory
            if (command.length() >= 5 && command.substring(0, 5).compareTo("index") == 0) {
                // TO-DO implement index operation
                // call the index method on the serve side engine and pass the folder to be indexed
                continue;
            }

            // if the command begins with search, search for files that matches the query
            if (command.length() >= 6 && command.substring(0, 6).compareTo("search") == 0) {
                // TO-DO implement index operation
                // extract the terms and call the server side engine method to search the terms for files
                continue;
            }

            System.out.println("unrecognized command!");
        }

        sc.close();
    }
}
