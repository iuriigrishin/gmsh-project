#include <gmsh.h>
#include <iostream>

int main() {
    gmsh::initialize();
    gmsh::model::add("test");
    std::cout << "Gmsh works!" << std::endl;
    gmsh::finalize();
    return 0;
}
