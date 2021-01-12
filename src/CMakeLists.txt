# Required CMake version
cmake_minimum_required(VERSION 3.0)

# Project name
project(aho_corasick)

set(SRCS
    aho_corasick/main.cpp
    )
    
set(HEADERS
    aho_corasick/file_to_buffer.hh
    aho_corasick/lexico_node.hh
    aho_corasick/lexicographic_tree.hh
    )

add_executable(aho_corasick ${SRCS} ${HEADERS})
