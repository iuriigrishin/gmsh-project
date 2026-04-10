cmake_minimum_required(VERSION 3.14)
project(microproject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(microproject
    src/main.cpp
    src/core/grid.cpp
    src/core/solver.cpp
    src/render/renderer.cpp
    src/scenes/block_melt.cpp
)

target_include_directories(microproject PRIVATE src)
target_link_libraries(microproject raylib)