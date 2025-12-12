## FileRetrievalEngine
**CSC435 Programming Assignment 4 (Winter 2024)**

**Jarvis College of Computing and Digital Media - DePaul University**

**Student**: Kedar Chandulur (kchandul@depaul.edu)  
**Solution programming language**: C++

### Requirements

If you are implementing your solution in C++ you will need to have GCC 12.x and CMake 3.22.x installed on your system. You will also need to install the C/C++ ZeroMQ libraries and development files. On Ubuntu 22.04 you can install GCC, CMake and ZeroMQ, and set GCC as default compiler using the following commands:

```
sudo apt install g++-12 gcc-12 cmake
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 110
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 120
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 110
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 120
sudo apt install libzmq3-dev libzmq5
sudo apt install libczmq-dev libczmq4
git submodule init
git submodule update
```

If you are implementing your solution in Java you will need to have Java 1.7.x and Maven 3.6.x installed on your systems. You will also need to install the JeroMQ (Java ZeroMQ) library and development jar. On Ubuntu 22.04 you can install Java, Maven and JeroMQ using the following commands:

```
sudo apt install openjdk-17-jdk maven libjeromq-java

```

### Setup

There are 5 datasets (Dataset1, Dataset2, Dataset3, Dataset4, Dataset5) that you need to use to evaluate your solution. Before you can evaluate your solution you need to download the datasets. You can download the datasets from the following link:

https://depauledu-my.sharepoint.com/:f:/g/personal/aorhean_depaul_edu/EgmxmSiWjpVMi8r6QHovyYIB-XWjqOmQwuINCd9N_Ppnug?e=TLBF4V

After you finished downloading the datasets copy them to the dataset directory (create the directory if it does not exist). Here is an example on how you can copy Dataset1 to the remote machine and how to unzip the dataset:

```
remote-computer$ mkdir datasets
local-computer$ scp Dataset1.zip cc@<remote-ip>:<path-to-repo>/datasets/.
remote-computer$ cd <path-to-repo>/datasets
remote-computer$ unzip Dataset1.zip
```

### C++ solution
#### How to build/compile

To build the C++ solution use the following commands:
```
cd app-cpp
mkdir build
cmake -S . -B build
cmake --build build
```

#### How to run applications

To run the C++ server (after you build the project) use the following command:
```
./build/file-retrieval-server <IP address> <port> <num workers>
> <list | quit>
```

To run the C++ client (after you build the project) use the following command:
```
./build/file-retrieval-client
> <connect | index | search | quit>
```

#### Example (2 clients and 1 server)

**Step 1:** start the server:

Server
```
./build/file-retrieval-server 127.0.0.1 12345 2
>
```

**Step 2:** start the clients and connect them to the server:

Client 1
```
./build/file-retrieval-client
> connect 127.0.0.1 12345
Connection successful!
```

Client 2
```
./build/file-retrieval-client
> connect 127.0.0.1 12345
Connection successful!
```

**Step 3:** list the connected clients on the server:

Server
```
> list
client1: 127.0.0.1
client2: 127.0.0.1
```

**Step 4:** index files from the clients:

Client 1
```
> index ../datasets/Dataset1/folder1
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder3
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder5
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder7
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder9
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder11
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder13
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder15
Completed indexing in 1.386 seconds
```

Client 2
```
> index ../datasets/Dataset1/folder2
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder4
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder6
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder8
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder10
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder12
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder14
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder16
Completed indexing in 1.386 seconds
```

**Step 5:** search files from the clients:

Client 1
```
> search Worms
Search completed in 2.8 seconds
Search results (top 10):
* client2:folder6/document200.txt 11
* client2:folder14/document417.txt 4
* client2:folder6/document424.txt 4
* client1:folder11/document79.txt 1
* client2:folder12/document316.txt 1
* client1:folder13/document272.txt 1
* client1:folder13/document38.txt 1
* client1:folder15/document351.txt 1
* client1:folder1/document260.txt 1
* client2:folder4/document101.txt 1
```

Client 2
```
> search distortion AND adaptation
Search completed in 3.27 seconds
Search results (top 10):
* client2:folder6/document200.txt 57
* client1:folder7/document476.txt 5
* client1:folder13/document38.txt 4
* client2:folder6/document408.txt 3
* client1:folder7/document298.txt 3
* client2:folder10/document107.txt 2
* client2:folder10/document206.txt 2
* client2:folder10/document27.txt 2
* client2:folder14/document145.txt 2
* client1:folder15/document351.txt 2
> quit
```

**Step 6:** close and disconnect the clients:

Client 1
```
> quit
```

Client 2
```
> quit
```

**Step 7:** close the server:

Server
```
> quit
```

### Java solution
#### How to build/compile

To build the Java solution use the following commands:
```
cd app-java
mvn compile
mvn package
```

#### How to run application

To run the Java server (after you build the project) use the following command:
```
java -cp target/app-java-1.0-SNAPSHOT-jar-with-dependencies.jar csc435.app.FileRetrievalServer <IP address> <port> <num workers>
```

To run the Java client (after you build the project) use the following command:
```
java -cp target/app-java-1.0-SNAPSHOT-jar-with-dependencies.jar csc435.app.FileRetrievalClient
```

#### Example (2 clients and 1 server)

**Step 1:** start the server:

Server
```
java -cp target/app-java-1.0-SNAPSHOT-jar-with-dependencies.jar csc435.app.FileRetrievalServer 127.0.0.1 12345 2
>
```

**Step 2:** start the clients and connect them to the server:

Client 1
```
java -cp target/app-java-1.0-SNAPSHOT-jar-with-dependencies.jar csc435.app.FileRetrievalClient
> connect 127.0.0.1 12345
Connection successful!
```

Client 2
```
java -cp target/app-java-1.0-SNAPSHOT-jar-with-dependencies.jar csc435.app.FileRetrievalClient
> connect 127.0.0.1 12345
Connection successful!
```

**Step 3:** list the connected clients on the server:

Server
```
> list
client1: 127.0.0.1
client2: 127.0.0.1
```

**Step 4:** index files from the clients:

Client 1
```
> index ../datasets/Dataset1/folder1
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder3
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder5
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder7
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder9
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder11
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder13
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder15
Completed indexing in 1.386 seconds
```

Client 2
```
> index ../datasets/Dataset1/folder2
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder4
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder6
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder8
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder10
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder12
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder14
Completed indexing in 1.386 seconds
> index ../datasets/Dataset1/folder16
Completed indexing in 1.386 seconds
```

**Step 5:** search files from the clients:

Client 1
```
> search Worms
Search completed in 2.8 seconds
Search results (top 10):
* client2:folder6/document200.txt 11
* client2:folder14/document417.txt 4
* client2:folder6/document424.txt 4
* client1:folder11/document79.txt 1
* client2:folder12/document316.txt 1
* client1:folder13/document272.txt 1
* client1:folder13/document38.txt 1
* client1:folder15/document351.txt 1
* client1:folder1/document260.txt 1
* client2:folder4/document101.txt 1
```

Client 2
```
> search distortion AND adaptation
Search completed in 3.27 seconds
Search results (top 10):
* client2:folder6/document200.txt 57
* client1:folder7/document476.txt 5
* client1:folder13/document38.txt 4
* client2:folder6/document408.txt 3
* client1:folder7/document298.txt 3
* client2:folder10/document107.txt 2
* client2:folder10/document206.txt 2
* client2:folder10/document27.txt 2
* client2:folder14/document145.txt 2
* client1:folder15/document351.txt 2
> quit
```

**Step 6:** close and disconnect the clients:

Client 1
```
> quit
```

Client 2
```
> quit
```

**Step 7:** close the server:

Server
```
> quit
```
