cmake_minimum_required(VERSION 3.16)

project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
)

FetchContent_MakeAvailable(SFML)

add_executable(PhysicsEngine
    src/main.cpp
    src/PhysicsObject.cpp

)

target_link_libraries(PhysicsEngine PRIVATE
    SFML::Graphics
)