cmake_minimum_required(VERSION 3.12)
project(hw4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(lib)
add_subdirectory(testing)

add_executable(hw4 main.cpp)

target_link_libraries(${PROJECT_NAME} huffman)