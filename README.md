# FileRetrievalEngine

A distributed, multi-client/server file indexing and search system built for a Distributed Systems course.  
It focuses on application layering, multithreaded indexing, and query processing over large text datasets.

## Overview

The engine follows a layered architecture (AppInterface, ProcessingEngine, IndexStore).  
Indexing uses multiple worker threads to speed up processing on large datasets, and search supports AND queries.


## Requirements

You will need to have GCC 12.x and CMake 3.22.x installed on your system. On Ubuntu 22.04 you can install GCC and set it as default compiler using the following commands:

```
sudo apt install g++-12 gcc-12 cmake
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 110
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 120
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 110
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 120
```

## Dataset setup

This project is evaluated using 5 Gutenberg-style datasets (Dataset1–Dataset5), each containing 16 folders of TXT documents.  
Do not commit datasets (raw/cleaned/intermediate outputs) to GitHub.

Please contact me for the datasets link.

After downloading, place datasets under a `datasets/` directory at the repository root.
Here is an example on how you can copy Dataset1 to the remote machine and how to unzip the dataset:

```
remote-computer$ mkdir datasets
local-computer$ scp Dataset1.zip cc@<remote-ip>:<path-to-repo>/datasets/.
remote-computer$ cd <path-to-repo>/datasets
remote-computer$ unzip Dataset1.zip
```

### Custom command

The server command is **./build/file-retrieval-server 127.0.0.1 12345**.


**./build/file-retrieval-client 1 16 127.0.0.1 12345**
I have implemented a range-based folder fetching functionality, I used this to make the testing easier.
So, the second argument 1 represents the first folder(starting folder of the range) and the 16 represents the last folder of the range.

If you use the above command for example like - **./build/file-retrieval-client 1 3 127.0.0.1 12345**, it starts the client with that range and when the path is provided on the index command,
like for example index **../datasets/Dataset1**, then the folders that get selected for the indexing are **../datasets/Dataset1/folder1/**, **../datasets/Dataset1/folder2/**, **../datasets/Dataset1/folder3/**.

The second and third parameters are address and port.

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
./build/file-retrieval-server
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
./build/file-retrieval-server
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
client1: 127.0.0.1 5746
client2: 127.0.0.1 9677
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


### Stats
<img width="724" height="668" alt="image" src="https://github.com/user-attachments/assets/c1fb8c00-6e8a-441d-b779-87530c40542f" />
<img width="890" height="671" alt="image" src="https://github.com/user-attachments/assets/743eceb4-4bd0-4857-bd65-4bb1065b586c" />
