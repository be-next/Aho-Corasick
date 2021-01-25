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
cmake_minimum_required(VERSION 3.7...3.19)


# Project name
project(aho_corasick VERSION 0.1
                     DESCRIPTION "Aho & Corasick algortithm"
                     LANGUAGES CXX)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(./src/aho_corasick ./src/util)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS ./src/main.cpp)

# is that useful ?    
set(HEADERS
    ./src/util/file_to_buffer.hh
    ./src/aho_corasick/lexico_node.hh
    ./src/aho_corasick/lexicographic_tree.hh
    )


add_executable(aho_corasick ${SRCS} ${HEADERS})

enable_testing()
file(COPY ./test/essai2 DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
add_test(NAME test1 WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}${CMAKE_BUILD_TYPE} COMMAND aho_corasick a ab bab bc bca c caa essai2)

