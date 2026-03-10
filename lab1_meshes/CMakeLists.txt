cmake_minimum_required(VERSION 3.10)
project(lab1_meshes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к Gmsh (под macOS + Homebrew; поправь под свою систему при необходимости)
set(GMSH_INCLUDE_DIR "/opt/homebrew/include")
set(GMSH_LIBRARY_DIR "/opt/homebrew/lib")

include_directories("${GMSH_INCLUDE_DIR}")
link_directories("${GMSH_LIBRARY_DIR}")

# ===== Лаба 1: тор =====
add_executable(lab1_torus
  torus/main.cpp
)

target_link_libraries(lab1_torus PRIVATE gmsh)

# ===== Лаба 1: STL =====
add_executable(lab1_stl
  stl/main.cpp
)

set_target_properties(lab1_torus PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
set_target_properties(lab1_stl PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(lab1_stl PRIVATE gmsh)

# При желании можно добавить ещё цели:
# add_executable(sample01 sample01.cpp)
# target_link_libraries(sample01 PRIVATE gmsh)
