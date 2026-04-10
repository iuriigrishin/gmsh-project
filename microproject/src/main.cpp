#include <raylib.h>
#include "core/grid.hpp"
#include "render/renderer.hpp"
#include "core/solver.hpp" 
#include "scenes/block_melt.hpp"

int main() {
    // parameters
    constexpr int    GRID_SIZE    = 200;
    constexpr double DOMAIN_SIZE  = 0.1;
    constexpr int    WINDOW_SIZE  = 1000;
    constexpr double DT           = 1e-6;
    constexpr int STEPS_PER_FRAME = 50;

    // initinilization
    sim::SimMaterial mat;
    Grid grid(GRID_SIZE, DOMAIN_SIZE, mat);

    BlockMelt scene;
    scene.init(grid);

    Solver solver(grid);
    Renderer renderer(WINDOW_SIZE);

    InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Stefan Problem");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        for (int s = 0; s < STEPS_PER_FRAME; ++s) { 
            solver.step(grid, DT);
        }    
        BeginDrawing();
        ClearBackground(BLACK);
        renderer.draw(grid);

        DrawText(scene.name().c_str(), 10, 10, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
