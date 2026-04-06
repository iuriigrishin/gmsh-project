cmake_minimum_required(VERSION 3.14)
project(stefan_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#raylib
include(FetchContent)
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.0
)
FetchContent_MakeAvailable(raylib)

# --- executable ---
add_executable(stefan_sim
    src/main.cpp
    src/core/grid.cpp
#   src/render/renderer.cpp
#   src/scenes/block_melt.cpp
)

# target_include_directories(stefan_sim PRIVATE src)
# target_link_libraries(stefan_sim raylib)
