#pragma once

#include <raylib.h>
#include <algorithm>
#include <cmath>

inline Color temperature_to_color(double T, double T_min, double T_max) {
    double t = (T - T_min) / (T_max - T_min);
    t = std::clamp(t, 0.0, 1.0);

    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    if (t < 0.5) {
        double s = t / 0.5;
        r = static_cast<unsigned char>(s * 255);
        g = static_cast<unsigned char>(s * 255);
        b = 255;
    } else {
        double s = (t - 0.5) / 0.5;
        r = 255;
        g = static_cast<unsigned char>((1.0 - s) * 255);
        b = static_cast<unsigned char>((1.0 - s) * 255);
    }

    return Color{r, g, b, 255};
}


