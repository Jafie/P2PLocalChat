cmake_minimum_required(VERSION 3.8)

set(SRCS
P2PLocalMessageLib.cpp
)

add_library(P2PLocalMessageLib ${SRCS} )

target_include_directories(P2PLocalMessageLib
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(P2PLocalMessageLib PRIVATE
CommunicationService
DatabaseService
)
