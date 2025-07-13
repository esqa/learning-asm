cmake_minimum_required(VERSION 3.28)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Ofast)

add_executable(untitled1 source/main/main.cpp
               source/main/main.h
               source/helper/helper.cpp
               source/helper/helper.h
               source/helper/helper.cpp
               source/main/main.cpp)
