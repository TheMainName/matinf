cmake_minimum_required(VERSION 4.0)
project(radixSort)

set(CMAKE_CXX_STANDARD 20)

add_executable(radixSort rsort.cpp main.cpp
        rsort.h)
