cmake_minimum_required(VERSION 3.8)

find_package(GTest)

set(SRCS
CommunicationServiceUnitTest.cpp
)

add_executable(CommunicationServiceUnitTest ${SRCS} )

target_link_libraries(CommunicationServiceUnitTest
PRIVATE
CommunicationService
GTest::GTest
)