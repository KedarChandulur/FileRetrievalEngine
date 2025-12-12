[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/hqM0T4I-)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=13724761&assignment_repo_type=AssignmentRepo)
## CSC435 Programming Assignment 2 (Winter 2024)
**Jarvis College of Computing and Digital Media - DePaul University**

**Student**: Kedar Chandulur (kchandul@depaul.edu)  
**Solution programming language**: C++

### Custom command
For running a search on 100 random elements.  
Make a text file and Add 100 words. Each word is in a new line. Make sure you add a minimum of 100 words.
To run the search on the text file,  
Use the command s100 <path-to-text-file>  
For example: s100 ../test/queries.txt

Note: I took the reference of words from here: https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1102/assignments/dictionary.txt

### Requirements

If you are implementing your solution in C++ you will need to have GCC 12.x and CMake 3.22.x installed on your system. On Ubuntu 22.04 you can install GCC and set it as default compiler using the following commands:

```
sudo apt install g++-12 gcc-12 cmake
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 110
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 120
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 110
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 120
```

If you are implementing your solution in Java you will need to have Java 1.7.x and Maven 3.6.x installed on your systems. On Ubuntu 22.04 you can install Java and Maven using the following commands:

```
sudo apt install openjdk-17-jdk maven

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

#### How to run application

To run the C++ solution (after you build the project) use the following command:
```
./build/file-retrieval-engine
> <index | search | quit>
```

#### Example

```
./build/file-retrieval-engine
> index ../datasets/Dataset1
Completed indexing in 10.386 seconds
> search Worms
Search completed in 2.8 seconds
Search results (top 10):
* folder6/document200.txt 11
* folder14/document417.txt 4
* folder6/document424.txt 4
* folder11/document79.txt 1
* folder12/document316.txt 1
* folder13/document272.txt 1
* folder13/document38.txt 1
* folder15/document351.txt 1
* folder1/document260.txt 1
* folder4/document101.txt 1
> search distortion AND adaptation
Search completed in 3.27 seconds
Search results (top 10):
* folder6/document200.txt 57
* folder7/document476.txt 5
* folder13/document38.txt 4
* folder6/document408.txt 3
* folder7/document298.txt 3
* folder10/document107.txt 2
* folder10/document206.txt 2
* folder10/document27.txt 2
* folder14/document145.txt 2
* folder15/document351.txt 2
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

To run the Java solution (after you build the project) use the following command:
```
java -cp target/app-java-1.0-SNAPSHOT.jar csc435.app.FileRetrievalEngine
```

#### Example

```
java -cp target/app-java-1.0-SNAPSHOT.jar csc435.app.FileRetrievalEngine
> index ../datasets/Dataset1
Completed indexing in 10.386 seconds
> search Worms
Search completed in 2.8 seconds
Search results (top 10):
* folder6/document200.txt 11
* folder14/document417.txt 4
* folder6/document424.txt 4
* folder11/document79.txt 1
* folder12/document316.txt 1
* folder13/document272.txt 1
* folder13/document38.txt 1
* folder15/document351.txt 1
* folder1/document260.txt 1
* folder4/document101.txt 1
> search distortion AND adaptation
Search completed in 3.27 seconds
Search results (top 10):
* folder6/document200.txt 57
* folder7/document476.txt 5
* folder13/document38.txt 4
* folder6/document408.txt 3
* folder7/document298.txt 3
* folder10/document107.txt 2
* folder10/document206.txt 2
* folder10/document27.txt 2
* folder14/document145.txt 2
* folder15/document351.txt 2
> quit
```
