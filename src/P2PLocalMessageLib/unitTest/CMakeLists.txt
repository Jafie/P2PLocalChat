cmake_minimum_required(VERSION 3.8)

find_package(GTest)

set(SRCS
P2PLocalMessageLibUnitTest.cpp
)

add_executable(P2PLocalMessageUnitTest ${SRCS} )

target_link_libraries(P2PLocalMessageUnitTest
PRIVATE
P2PLocalMessageLib
GTest::GTest
)