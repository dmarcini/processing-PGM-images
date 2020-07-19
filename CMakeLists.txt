cmake_minimum_required(VERSION 3.10)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(processing-PGM-images)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(include)

set(SOURCES src/main.cpp
            src/image.cpp
            src/image_operations.cpp)

add_executable(processing-PGM-images ${SOURCES})

target_link_libraries(processing-PGM-images /usr/local/lib/libutility.a
                                            /usr/local/lib/libmath.a)
