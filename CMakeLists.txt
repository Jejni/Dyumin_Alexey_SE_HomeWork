cmake_minimum_required(VERSION 3.6)
project(Hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Hw1 ${SOURCE_FILES})
include_directories(cereal)