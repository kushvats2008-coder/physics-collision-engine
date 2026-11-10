cmake_minimum_required(VERSION 3.16)

project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# 1. Fetch SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
)

# 2. Fetch JSON via Git
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(SFML json)

add_executable(PhysicsEngine
    src/main.cpp
    src/PhysicsObject.cpp
    src/InputHandler.cpp
    src/savemanager.h
    src/savemanager.cpp
)

target_link_libraries(PhysicsEngine PRIVATE
    SFML::Graphics
    nlohmann_json::nlohmann_json
)