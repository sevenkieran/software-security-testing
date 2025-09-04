cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(leak_detector VERSION 1.0)
add_executable(leak_detector leak_detector.c)
