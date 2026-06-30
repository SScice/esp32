#include "display_fb.h"
#include <Arduino.h>
#include <cstring>

Framebuffer::Framebuffer(uint8_t* black_plane, uint8_t* red_plane)
    : black(black_plane), red(red_plane) {}

void Framebuffer::clear(uint8_t paper) {
    if (!black || !red) return;
    uint8_t b = (paper == layout::COLOR_BLACK) ? 0x00 : 0xFF;
    memset(black, b, layout::PLANE_BYTES);
    memset(red, 0x00, layout::PLANE_BYTES);
}

void Framebuffer::setPixel(int x, int y, uint8_t color) {
    if (x < 0 || y < 0 || x >= layout::PANEL_W || y >= layout::PANEL_H) return;
    const int idx = y * (layout::PANEL_W / 8) + (x >> 3);
    const uint8_t mask = 0x80 >> (x & 7);
    if (color == layout::COLOR_RED) {
        black[idx] |= mask;
        red[idx] &= ~mask;
    } else if (color == layout::COLOR_BLACK) {
        black[idx] &= ~mask;
        red[idx] |= mask;
    } else {
        black[idx] |= mask;
        red[idx] |= mask;
    }
}

void Framebuffer::fillRect(int x, int y, int w, int h, uint8_t color) {
    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++)
            setPixel(x + i, y + j, color);
}

void Framebuffer::drawHLine(int x, int y, int w, uint8_t color) {
    fillRect(x, y, w, 1, color);
}

void Framebuffer::blit1bpp(int dx, int dy, int w, int h, const uint8_t* src, uint8_t fg, uint8_t bg) {
    if (!src) return;
    const int row_bytes = (w + 7) / 8;
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            const int sidx = row * row_bytes + (col >> 3);
            const uint8_t on = (src[sidx] & (0x80 >> (col & 7))) != 0;
            setPixel(dx + col, dy + row, on ? fg : bg);
        }
    }
}