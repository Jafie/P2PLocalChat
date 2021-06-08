# Install a build chain for the P2PLocalMessage system
FROM ubuntu:20.04

# Create build chain
RUN apt-get update
RUN DEBIAN_FRONTEND="noninteractive" apt-get -y install sqlite3 libsqlite3-dev build-essential git cmake libzmq3-dev libgtest-dev

# Install cppzmq
RUN git clone https://github.com/zeromq/cppzmq.git
RUN cd cppzmq && mkdir build && cd build && cmake -DCPPZMQ_BUILD_TESTS=OFF .. && make install

# Return to root path and install source code
RUN mkdir -p P2PLocalMessage/src
ADD src P2PLocalMessage/src
ADD CMakeLists.txt P2PLocalMessage
ADD README.md P2PLocalMessage