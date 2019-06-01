cmake_minimum_required(VERSION 2.8)

project(BIGINT_HW3)

include_directories(${BIGINT_HW3_SOURCE_DIR})

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_BUILD_TYPE Release)

add_executable(big_integer_testing
        big_integer_testing.cpp
        uintvector.h
        uintvector.cpp
        big_integer.h
        big_integer.cpp
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(big_integer_testing -lpthread)
