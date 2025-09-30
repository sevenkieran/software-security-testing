cmake_minimum_required(VERSION 3.28)
project(MemoryLeakDetector)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_C_STANDARD 17)

set(SOURCE_FILES
        src/deprecated_header.c
        src/main.c
        src/leak_detector.c

        include/analyze.h
        src/analyze.c
        include/argparse.h
        src/argparse.c
        include/utils.h
        src/utils.c
        include/constants.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})