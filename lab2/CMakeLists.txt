cmake_minimum_required(VERSION 3.27)

project(lab2 CXX)

SET(CMAKE_CXX_STANDARD 23)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES
        source/main.cpp
        source/TransportTask.cpp
        source/utility/print_table.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)