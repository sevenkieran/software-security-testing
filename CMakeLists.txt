cmake_minimum_required(VERSION 3.28)
project(MemoryLeakDetector)
set(CMAKE_C_STANDARD 17)

set(SOURCE_FILES src/

        src/deprecatedHeader.c
        src/main.c
        src/leakdetector2.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})