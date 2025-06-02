cmake_minimum_required(VERSION 3.24)

project(coursework)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_TLS_VERIFY OFF)

SET(SOURCE_FILES
        source/main.cpp
        source/Graph.cpp
        source/TSPSolver/TSPSolverBrute.cpp
        source/TSPSolver/TSPSolverDP.cpp
        source/TSPSolver/TSPSolverGenetic.cpp
)

include_directories(source)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)