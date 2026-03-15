cmake_minimum_required(VERSION 3.12)
project(PencilSimulation)

set(CMAKE_CXX_STANDARD 17)

find_package(VTK REQUIRED)

# Gmsh — ищем вручную через pkg-config или стандартный путь
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(GMSH gmsh)
endif()

if(NOT GMSH_FOUND)
    # Fallback: homebrew путь на маке
    set(GMSH_INCLUDE_DIRS /opt/homebrew/include)
    set(GMSH_LIBRARIES /opt/homebrew/lib/libgmsh.dylib)
endif()

add_executable(main main.cpp)

target_include_directories(main PRIVATE ${GMSH_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${VTK_LIBRARIES} ${GMSH_LIBRARIES})

vtk_module_autoinit(
    TARGETS main
    MODULES ${VTK_LIBRARIES}
)