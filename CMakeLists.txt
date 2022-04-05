cmake_minimum_required(VERSION 3.10)

# set the project name
project(popdb)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
# add the executable
add_executable(popdb main.cpp ${SRC_FILES})
target_include_directories(popdb PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")