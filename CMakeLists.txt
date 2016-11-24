cmake_minimum_required(VERSION 3.6)
project(Hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp include/default_info.cpp include/default_info.h include/user.cpp include/user.h include/book.cpp include/book.h include/library.cpp include/library.h include/controller.cpp include/controller.h include/view.cpp include/view.h data_base.cpp)
add_executable(Hw1 ${SOURCE_FILES} )
include_directories("${PROJECT_SOURCE_DIR}/include")
