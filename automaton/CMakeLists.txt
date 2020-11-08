project(automaton_lib)
cmake_minimum_required(VERSION 3.8)


set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "--coverage")


set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})


include_directories(lib practice1)

add_executable(main practice1/practice1_variant5.cpp)

add_executable(tests tests/practice1/tests.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES} Threads::Threads)

