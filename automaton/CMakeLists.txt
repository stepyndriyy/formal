project(automaton_lib)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(lib tests)



add_executable(main minimalize.cpp)
add_executable(hw4_n1 hw4_n1.cpp)
add_executable(hw4_n2 hw4_n2.cpp)
add_executable(Test test.cpp)

target_link_libraries(
  Test ${GTEST_LIBRARIES} Threads::Threads)

enable_testing()
add_test(Test "./Test")
