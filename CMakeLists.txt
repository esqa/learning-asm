cmake_minimum_required(VERSION 3.28)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Ofast)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        source/*.cpp
        source/*.h
)

add_executable(untitled1 ${SOURCES})