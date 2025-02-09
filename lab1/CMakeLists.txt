cmake_minimum_required(VERSION 3.27)

project(lab1)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
        source/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})