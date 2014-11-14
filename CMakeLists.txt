cmake_minimum_required(VERSION 2.8.4)
project(cpp_cs_hz_2_huffman)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp huffman.cpp)
add_executable(cpp_cs_hz_2_huffman ${SOURCE_FILES})