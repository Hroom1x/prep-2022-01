cmake_minimum_required(VERSION 3.6)
project(homework)

set(CMAKE_CXX_STANDARD 17)

#Set your directories.  The dot representes the root application folder.
#Thus my the path to my domain folder:
set(APP_SRC vkhw/project/src)
set(APP_HEADS vkhw/project/include)

#Include the directories and now your cpp files will recognize your headers
include_directories(${APP_SRC})
include_directories(${APP_HEADS})
include_directories(../)

set(
        SOURCE_FILES
        ./src/list.hpp
        ./test/test.cpp
)
add_executable(untitled ${SOURCE_FILES})
