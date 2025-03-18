cmake_minimum_required(VERSION 3.27)

project(lab3 CXX)

SET(CMAKE_CXX_STANDARD 23)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(SOURCE_FILES
        source/main.cpp
        source/alg/AlgsForExtremes.cpp
        source/BaseTask.cpp
        source/PlotTask.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})