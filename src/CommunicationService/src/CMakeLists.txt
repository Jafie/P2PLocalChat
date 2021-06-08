cmake_minimum_required(VERSION 3.8)

find_package(cppzmq)

set(SRCS
CommunicationService.cpp
MessageReceiver.cpp
MessageSender.cpp
)

add_library(CommunicationService ${SRCS} )

target_include_directories(CommunicationService
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(CommunicationService
PUBLIC
cppzmq
)