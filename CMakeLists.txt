cmake_minimum_required(VERSION 3.31)
project(biscuit)


# flags
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# src
file(GLOB_RECURSE SRCS src/*.cpp src/*.c)
file(GLOB_RECURSE HEADERS include/*)

add_executable(main ${SRCS})

# include
include_directories(${CMAKE_SOURCE_DIR}/include)



# tests
if (DEFINED BSQT_TESTS)
    set(SRCS_WITHOUT_MAIN "")
    foreach(file ${SRCS})
        if(NOT "${file}" MATCHES "src/main\\.c")
            list(APPEND SRCS_WITHOUT_MAIN ${file})
        endif()
    endforeach()
    list(APPEND SRCS_WITHOUT_MAIN tests/bsqt_tester.c)

    add_executable(bsqt_tests_bin ${SRCS_WITHOUT_MAIN})
    file(COPY "${CMAKE_SOURCE_DIR}/tests/bsqt_tests.py" DESTINATION "${CMAKE_BINARY_DIR}")
endif()
