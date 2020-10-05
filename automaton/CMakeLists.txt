project(automaton_lib)
cmake_minimum_required(VERSION 3.16)


set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(lib)

add_executable(main minimalize.cpp)
add_executable(hw4_n1 hw4_n1.cpp)
add_executable(hw4_n2 hw4_n2.cpp)
