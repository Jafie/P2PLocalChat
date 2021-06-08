cmake_minimum_required(VERSION 3.8)

set(SRCS
DatabaseService.cpp
)

add_library(DatabaseService ${SRCS} )

target_include_directories(DatabaseService
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(DatabaseService sqlite3)
