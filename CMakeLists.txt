cmake_minimum_required(VERSION 3.28)
project(MemoryLeakDetector)
set(CMAKE_C_STANDARD 17)

set(SOURCE_FILES src/leak_detector.c
        test/leak.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})