cmake_minimum_required(VERSION 3.31)
project(biscuit)


# flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

# src
file(GLOB_RECURSE SRCS src/*.cpp src/*.c)
file(GLOB_RECURSE HEADERS fe-headers/* include/*)

add_executable(main ${SRCS})

# include
include_directories(${CMAKE_SOURCE_DIR}/include)
