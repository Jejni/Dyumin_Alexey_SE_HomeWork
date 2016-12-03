cmake_minimum_required(VERSION 3.6)
project(Hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp library/types/DefaultInfo.cpp library/types/DefaultInfo.h library/types/User.cpp library/types/User.h library/types/Book.cpp library/types/Book.h library/Library.cpp library/Library.h library/Controller.cpp library/Controller.h library/View.cpp library/View.h library/database/DatabaseSerial.cpp library/database/DatabaseTextFile.cpp library/database/Database.h library/database/DatabaseTextFile.h)
add_executable(Hw1 ${SOURCE_FILES} )
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/library")