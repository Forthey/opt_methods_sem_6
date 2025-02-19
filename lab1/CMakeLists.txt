cmake_minimum_required(VERSION 3.27)

SET(CMAKE_CXX_STANDARD 23)

project(lab1)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
        source/main.cpp
        source/LinearTask.cpp
        source/SlackFormTask.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})