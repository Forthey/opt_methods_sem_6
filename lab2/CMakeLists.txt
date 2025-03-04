cmake_minimum_required(VERSION 3.27)

project(lab2 CXX)

SET(CMAKE_CXX_STANDARD 23)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES
        source/main.cpp
        source/TransportTask.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})