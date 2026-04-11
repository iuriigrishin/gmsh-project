#include <raylib.h>
#include "core/grid.hpp"
#include "render/renderer.hpp"
#include "scenes/block_melt.hpp"
#include "core/solver.hpp"
#include <cstdio>

int main() {
    // parametersя
    constexpr int    GRID_SIZE    = 100;
    constexpr double DOMAIN_SIZE = 0.01;
    constexpr int    WINDOW_SIZE  = 1000;
    constexpr double DT           = 1e-7;
    constexpr int STEPS_PER_FRAME = 100;

    // initinilization
    sim::SimMaterial mat;
    mat.k_solid  = 200.0;   // было 2.2
    mat.k_liquid = 200.0;   // было 0.6
    Grid grid(GRID_SIZE, DOMAIN_SIZE, mat);

    BlockMelt scene;
    scene.init(grid);

    Solver solver(grid, BoundaryType::Adiabatic);
    Renderer renderer(WINDOW_SIZE);

    InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Stefan Problem");
    SetTargetFPS(60);

    bool paused = false;
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }

        if (IsKeyPressed(KEY_R)) {
            scene.init(grid);
            solver = Solver(grid, BoundaryType::Adiabatic);
            paused = false;
        }

        if (!paused) {
            for (int s = 0; s < STEPS_PER_FRAME; ++s) {
                solver.step(grid, DT);
            }
        } 
        BeginDrawing();
        ClearBackground(BLACK);
        renderer.draw(grid);

        char info[128];
        std::snprintf(info, sizeof(info),
            "%s | t = %.4f s | step %d | %s",
            scene.name().c_str(),
            solver.time(),
            solver.steps(),
            paused ? "PAUSED" : "RUNNING"
        );
        DrawText(info, 10, 10, 18, WHITE);
        DrawText("[SPACE] pause  [R] reset", 10, WINDOW_SIZE - 30, 16, GRAY);
        DrawFPS(WINDOW_SIZE - 90, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
