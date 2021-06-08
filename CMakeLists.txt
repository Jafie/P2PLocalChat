cmake_minimum_required(VERSION 3.8)

project(P2PMessage)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -Wall)

add_subdirectory(src)

# Testing part
enable_testing()
add_test(DatabaseServiceTest src/DatabaseService/unitTest/DatabaseServiceUnitTest)
add_test(CommunicationServiceTest src/CommunicationService/unitTest/CommunicationServiceUnitTest)
add_test(P2PLocalMessage src/P2PLocalMessageLib/unitTest/P2PLocalMessageUnitTest)
