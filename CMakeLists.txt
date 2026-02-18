cmake_minimum_required(VERSION 3.10)
project(gmsh-project)

add_executable(myapp main.cpp)
target_link_libraries(myapp gmsh)
