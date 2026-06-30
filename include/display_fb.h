#pragma once

#include <cstdint>
#include "layout_design.h"

// MSB-first 1bpp: bit 0 = black pixel when plane bit clear (Waveshare black buffer convention).

class Framebuffer {
public:
    uint8_t* black;
    uint8_t* red;

    explicit Framebuffer(uint8_t* black_plane, uint8_t* red_plane);

    void clear(uint8_t paper = layout::COLOR_PAPER);
    void setPixel(int x, int y, uint8_t color);
    void fillRect(int x, int y, int w, int h, uint8_t color);
    void drawHLine(int x, int y, int w, uint8_t color);
    void blit1bpp(int dx, int dy, int w, int h, const uint8_t* src, uint8_t fg, uint8_t bg);
};