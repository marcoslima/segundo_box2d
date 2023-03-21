#pragma once

#include <algorithm>
#include <math.h>
#include <iostream>

void rgb_to_hls(float r, float g, float b, float &h, float &l, float &s) {
    float cmax, cmin, delta;
    cmax = std::max({r, g, b});
    cmin = std::min({r, g, b});
    delta = cmax - cmin;
    l = (cmax + cmin) / 2;
    if (delta == 0) {
        h = 0;
        s = 0;
        return;
    }
    if (l <= 0.5) {
        s = delta / (cmax + cmin);
    }
    else {
        s = delta / (2 - cmax - cmin);
    }
    if (cmax == r) {
        h = 60 * fmod((g - b) / delta, 6);
    }
    else if (cmax == g) {
        h = 60 * ((b - r) / delta + 2);
    }
    else {
        h = 60 * ((r - g) / delta + 4);
    }
    if (h < 0) {
        h += 360;
    }
    h /= 360;
}

void hsl_to_rgb(float h, float s, float l, float &r, float &g, float &b) {
    if (s == 0.0) {
        r = g = b = l;
    } else {
        float q = (l < 0.5) ? (l * (1.0 + s)) : (l + s - l * s);
        float p = 2.0 * l - q;
        float hk = h / 6.0;
        float tr = hk + 1.0 / 3.0;
        float tg = hk;
        float tb = hk - 1.0 / 3.0;
        if (tr < 0.0) tr += 1.0;
        if (tr > 1.0) tr -= 1.0;
        if (tg < 0.0) tg += 1.0;
        if (tg > 1.0) tg -= 1.0;
        if (tb < 0.0) tb += 1.0;
        if (tb > 1.0) tb -= 1.0;
        r = (tr < 1.0 / 6.0) ? (p + ((q - p) * 6.0 * tr)) :
            ((tr < 0.5) ? q :
             ((tr < 2.0 / 3.0) ? (p + ((q - p) * 6.0 * (2.0 / 3.0 - tr))) : p));
        g = (tg < 1.0 / 6.0) ? (p + ((q - p) * 6.0 * tg)) :
            ((tg < 0.5) ? q :
             ((tg < 2.0 / 3.0) ? (p + ((q - p) * 6.0 * (2.0 / 3.0 - tg))) : p));
        b = (tb < 1.0 / 6.0) ? (p + ((q - p) * 6.0 * tb)) :
            ((tb < 0.5) ? q :
             ((tb < 2.0 / 3.0) ? (p + ((q - p) * 6.0 * (2.0 / 3.0 - tb))) : p));
    }
}
