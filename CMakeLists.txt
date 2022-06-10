cmake_minimum_required(VERSION 3.6)
project(homework)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCE_FILES
        ./src/list.hpp
        ./test/test.cpp
)
add_executable(untitled ${SOURCE_FILES})
