cmake_minimum_required(VERSION 3.12)
project(huffman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

set(SOURCE_LIB huffman_enc.cpp huffman_dec.cpp huffman_tree.cpp symbol_counter.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_LIB})
