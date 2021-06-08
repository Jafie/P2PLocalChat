# P2PMessage

P2PMessage is a simple communication system in P2P based on ZMQ (TCP protocol) in C++. Currently limited to local communication.

  - Register your "user name" and a "network".
  - Send messages to others "user name".
  - Enjoy!

# Compatibility

 Build and tests had been done on an Ubuntu 20.04 LTS.
 A DockerFile is available to configure your own build chain. (Tested with Windows 10).

# How to build

You can build your application with Docker or your own system. The application should be compatible under any systems based on Linux.
* *During this README, all examples had been tested on an Ubuntu 20.04 LTS. The Docker image itself is based on Ubuntu 20.04 LTS*

### Dependencies

* [[Sqlite3]](https://www.sqlite.org/index.html) - A C Library for SQL database engine.
* [[ZeroMQ]](https://zeromq.org/) -  An open-source universal messaging library.
* [[cppzmq]](https://github.com/zeromq/cppzmq) - Header-only C++ binding for libzmq.
* [[Google Test]](https://github.com/google/googletest) - great UI boilerplate for modern web apps
* [[CMake]](https://cmake.org/) - An extensible, open-source system that manages the build process.

Be careful, **a C++ compiler that supports at least the C++11 is necessary.**

On Ubuntu 20.04, you can install all **SQLite3, ZMQ CMake, and Google Test** with the following command:

```bash
$ sudo apt-get install sqlite3 libsqlite3-dev libzmq3-dev libgtest-dev cmake
```

**Concerning cppzmq**, you can install it on your system with the following procedure:

```bash
$ git clone https://github.com/zeromq/cppzmq.git
$ cd cppzmq
$ mkdir build && cd build
$ cmake -DCPPZMQ_BUILD_TESTS=OFF ..
$ make install
```

### Build instructions

If all dependencies are installed, you can build P2PMessage with the following command

```bash
$ git clone https://github.com/Jafie/P2PMessage.git
$ cd P2PMessage
$ mkdir build && cd build
$ cmake ..
$ make
```

### Build instruction (Provided docker image)

You can generate a "Docker Build Chain" with the following commands.

```bash
$ git clone https://github.com/Jafie/P2PMessage.git
$ cd P2PMessage
$ docker build -t p2pmsgbuild . 
$ docker run -t -i p2pmsgbuild
$ cd P2PLocalMessage
$ mkdir build && cd build
$ cmake ..
$ make
```

# How to launch

Then, your application P2PMessage is ready and located in the "build" directory. Based on a simple terminal IUI, you can launch it with the following command:

```bash
$ ./P2PLocalMessage
```
Or, with a defined SQLite3 database
```bash
$ ./P2PLocalMessage your_sqlite3_database_path
```

# Launch Unitary Test
A set of unitary tests is available to validate the application. In the "build" repository, launch:
```bash
$ make test
```

# Limitations

Due to local database usage, P2PMessage is not able to communicate outside of your own system (without shared folders…).
Moreover, for simplicity questions, message receptions are not notified.

# Documentation

### Simplified Class Diagram
![image](https://drive.google.com/uc?export=view&id=1SlHlg4HitzwoFzpKz2P-CtvdEDBf33sr)

### Explanation
The **P2PLocalMessageLib** is connected to two static libraries:

- **CommunicationService**: Responsible for communication in P2P through ZMQ.
- **DatabaseService**: Used to identify all users connected.

On P2PLocalMessageLibrary:
A **"initialization"** creates a "MessageSender" in the CommunicationLayer. Then, the DatabaseService adds a new entry [UserName/NetworkInfo] in the Database.

A **"collectMessage"** call will simply call the "collectMessage" from CommmunicationService. This process retrieves all the messages received and place it in a vector of strings.

Concerning **"sendMessage"** it will at first check if the user name exists in the CommunicationService map (cache). If not, it contacts the DatabaseService to retrieve network information. Then, if everything is OK, the message is sent to the corresponding MessageSender in CommunicationService.
