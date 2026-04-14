#include <raylib.h>
#include <cstdio>
#include "core/grid.hpp"
#include "core/solver.hpp"
#include "render/renderer.hpp"
#include "scenes/block_melt.hpp"
#include "scenes/stefan_1d.hpp"
#include "export/csv_writer.hpp"
#include "scenes/snowflake.hpp"

int main() {
    // parametersя
    constexpr int    GRID_SIZE       = 200;
    constexpr double DOMAIN_SIZE     = 0.1;
    constexpr int    WINDOW_SIZE     = 800;
    constexpr double DT              = 5e-4;
    constexpr int    STEPS_PER_FRAME = 20;
    constexpr int    CSV_INTERVAL    = 50;

    // initinilization
    sim::SimMaterial mat;
    Grid grid(GRID_SIZE, DOMAIN_SIZE, mat);

    BlockMelt scene_block; // для валидации
    StefanValidation scene_stefan;
    Scene* scene = &scene_stefan;
    scene->init(grid);


    Snowflake scene_snow;
    // для валидации: левая стенка фиксирована, остальные адиабатические
    Solver solver(grid, BoundaryType::Fixed);
    Renderer renderer(WINDOW_SIZE);
    CsvWriter csv("results/front.csv");

    InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Stefan Problem");
    SetTargetFPS(60);

    bool recording = false;
    int frame_count = 0;
    bool paused = false;
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) paused = !paused;

        if (IsKeyPressed(KEY_ONE)) {
            scene = &scene_block;
            scene->init(grid);
            solver = Solver(grid, BoundaryType::Adiabatic);
            paused = false;
        }
        if (IsKeyPressed(KEY_TWO)) {
            scene = &scene_stefan;
            scene->init(grid);
            solver = Solver(grid, BoundaryType::Fixed);
            paused = false;
        }
        if (IsKeyPressed(KEY_R)) {
            scene->init(grid);
            solver = Solver(grid, BoundaryType::Fixed);
            paused = false;
        }

        if (IsKeyPressed(KEY_THREE)) {
            scene = &scene_snow;
            scene->init(grid);
            solver = Solver(grid, BoundaryType::Adiabatic);
            paused = false;
        }

        if (!paused) {
            for (int s = 0; s < STEPS_PER_FRAME; ++s) {
                solver.step(grid, DT);

                // записываем фронт в CSV
                if (solver.steps() % CSV_INTERVAL == 0) {
                    csv.write_front(grid, solver.time());
                }
            }
        }

        // левая кнопка мыши — горячий источник
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 pos = GetMousePosition();
            int j = static_cast<int>(pos.x / (WINDOW_SIZE / GRID_SIZE));
            int i = static_cast<int>(pos.y / (WINDOW_SIZE / GRID_SIZE));
            int radius = 3;
            for (int di = -radius; di <= radius; ++di) {
                for (int dj = -radius; dj <= radius; ++dj) {
                    int ni = i + di, nj = j + dj;
                    if (ni >= 0 && ni < GRID_SIZE && nj >= 0 && nj < GRID_SIZE) {
                        if (di*di + dj*dj <= radius*radius) {
                            grid.H(ni, nj) = grid.H_from_T_liquid(80.0);
                        }
                    }
                }
            }
        }

        // правая кнопка мыши — холодный источник
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 pos = GetMousePosition();
            int j = static_cast<int>(pos.x / (WINDOW_SIZE / GRID_SIZE));
            int i = static_cast<int>(pos.y / (WINDOW_SIZE / GRID_SIZE));
            int radius = 3;
            for (int di = -radius; di <= radius; ++di) {
                for (int dj = -radius; dj <= radius; ++dj) {
                    int ni = i + di, nj = j + dj;
                    if (ni >= 0 && ni < GRID_SIZE && nj >= 0 && nj < GRID_SIZE) {
                        if (di*di + dj*dj <= radius*radius) {
                            grid.H(ni, nj) = grid.H_from_T_solid(-30.0);
                        }
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        renderer.draw(grid);

        char info[128];
        std::snprintf(info, sizeof(info),
            "%s | t = %.6f s | step %d | %s",
            scene->name().c_str(),
            solver.time(),
            solver.steps(),
            paused ? "PAUSED" : "RUNNING"
        );
        DrawText(info, 10, 10, 18, WHITE);
        DrawText("[1][2] scenes  [SPACE] pause  [R] reset  [F] record  [LMB] heat  [RMB] freeze", 10, WINDOW_SIZE - 30, 14, GRAY);
        DrawFPS(WINDOW_SIZE - 90, 10);

        EndDrawing();
        if (IsKeyPressed(KEY_F)) {
            recording = !recording;
            if (recording) frame_count = 0;
        }

        if (recording) {
            char path[128];
            std::snprintf(path, sizeof(path), "results/frame_%05d.png", frame_count);
            TakeScreenshot(path);
            frame_count++;
        }
    }

    CsvWriter::dump_temperature(grid, "results/temperature.csv");

    CloseWindow();
    return 0;
}