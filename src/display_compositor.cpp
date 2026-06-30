#include "display_compositor.h"
#include "calendar_render.h"
#include "custom_image.h"
#include "layout_design.h"
#include "clock_font.h"
#include "lunar.h"
#include <Arduino.h>

DisplayCompositor::DisplayCompositor() : black(nullptr), red(nullptr), fb(black, red) {}

DisplayCompositor::~DisplayCompositor() {
    if (black) free(black);
    if (red) free(red);
}

bool DisplayCompositor::allocate() {
    black = (uint8_t*)malloc(layout::PLANE_BYTES);
    red = (uint8_t*)malloc(layout::PLANE_BYTES);
    if (!black || !red) return false;
    fb = Framebuffer(black, red);
    return true;
}

void DisplayCompositor::drawBottomStrip(const struct tm* now) {
    fb.fillRect(0, layout::BOTTOM_Y, layout::PANEL_W, layout::BOTTOM_H, layout::COLOR_PAPER);
    fb.drawHLine(0, layout::BOTTOM_Y, layout::PANEL_W, layout::COLOR_BLACK);
    if (layout::BOTTOM_STRIP_BORDER_PX > 1)
        fb.drawHLine(0, layout::BOTTOM_Y + 1, layout::PANEL_W, layout::COLOR_BLACK);
    char lunar[layout::BOTTOM_STRIP_LINE_BUF];
    char clock[layout::BOTTOM_STRIP_LINE_BUF];
    formatLunarLine(lunar, sizeof(lunar), now);
    formatClockLine(clock, sizeof(clock), now);

    const int lunar_y = layout::BOTTOM_CLOCK_CENTER_Y - CLOCK_GLYPH_H / 2;
    if (lunar[0] != '\0')
        drawStripString(fb, layout::BOTTOM_LUNAR_X, lunar_y, lunar);
    drawClockString(fb, layout::BOTTOM_CLOCK_CENTER_X, layout::BOTTOM_CLOCK_CENTER_Y, clock);
}

void DisplayCompositor::composeFull(const struct tm* now) {
    fb.clear(layout::COLOR_PAPER);
    drawCalendarPanel(fb, now);
    blitCustomImage(fb);
    drawBottomStrip(now);
}

void DisplayCompositor::pushFrame() {
    epd.DisplayFramePlanes(black, red);
}