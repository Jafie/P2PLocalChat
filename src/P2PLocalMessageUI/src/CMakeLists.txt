cmake_minimum_required(VERSION 3.8)

set(SRCS
P2PLocalMessageUI.cpp
main.cxx
)

add_executable(P2PLocalMessage ${SRCS} )

target_link_libraries(P2PLocalMessage PRIVATE
P2PLocalMessageLib
)

add_custom_command(TARGET P2PLocalMessage 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:P2PLocalMessage> ${PROJECT_BINARY_DIR}
                   )
