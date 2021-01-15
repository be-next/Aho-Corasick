#
# To clean up copy of the repository, do :
#   git clean -d -f -x
# this command remove all files and directories created by cmake and build process
#
# On Mac OS, if you get this error:
#   No CMAKE_C_COMPILER could be found
# do
#   sudo xcode-select --reset
# I didn't try it, but if you installed CMake from HomeBrew, it sounds that the upgrade force the compiler to be reconfigured:
#   $ brew upgrade cmake


# Required CMake version
cmake_minimum_required(VERSION 3.0)

# Project name
project(aho_corasick)

include_directories(./src/aho_corasick)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS ./src/main.cpp)
    
set(HEADERS
    ./src/aho_corasick/file_to_buffer.hh
    ./src/aho_corasick/lexico_node.hh
    ./src/aho_corasick/lexicographic_tree.hh
    )

add_executable(aho_corasick ${SRCS} ${HEADERS})
