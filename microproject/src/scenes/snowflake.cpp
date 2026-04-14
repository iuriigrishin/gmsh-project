#include "scenes/snowflake.hpp"
#include <raylib.h>

void Snowflake::init(Grid& grid) {
    int n = grid.size();

    Image img = LoadImage("assets/image.png");
    ImageResize(&img, n, n);
    Color* pixels = LoadImageColors(img);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            Color c = pixels[i * n + j];
            double brightness = (c.r + c.g + c.b) / (3.0 * 255.0);

            if (brightness < 0.5) {
                grid.H(i, j) = grid.H_from_T_solid(-1.0);
            } else {
                grid.H(i, j) = grid.H_from_T_liquid(1.0);
            }
        }
    }

    UnloadImageColors(pixels);
    UnloadImage(img);
}