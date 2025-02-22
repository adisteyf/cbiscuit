cmake_minimum_required(VERSION 3.31)
project(biscuit)


# flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# src
file(GLOB_RECURSE SRCS src/*.cpp src/*.c)
file(GLOB_RECURSE HEADERS include/*)

add_executable(main ${SRCS})

# include
include_directories(${CMAKE_SOURCE_DIR}/include)
