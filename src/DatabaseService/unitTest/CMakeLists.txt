cmake_minimum_required(VERSION 3.8)

find_package(GTest)

set(SRCS
DatabaseServiceUnitTest.cpp
)

add_executable(DatabaseServiceUnitTest ${SRCS} )

target_link_libraries(DatabaseServiceUnitTest
DatabaseService
GTest::GTest
)
