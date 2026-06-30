#include "custom_image.h"
#include "display_fb.h"
#include "layout_design.h"

// Default placeholder art panel (replace custom_image.h with your C arrays for production art).
void blitCustomImage(Framebuffer& fb) {
    fb.fillRect(layout::ART_X, layout::ART_Y, layout::ART_W, layout::ART_H, layout::COLOR_PAPER);
    fb.drawHLine(layout::ART_X, layout::ART_Y + layout::ART_H - 1, layout::ART_W, layout::COLOR_BLACK);
    for (int i = 0; i < 10; i++) {
        const int y = layout::ART_Y + 16 + i * 28;
        fb.drawHLine(layout::ART_X + 12, y, layout::ART_W - 24,
                     (i % 3 == 0) ? layout::COLOR_RED : layout::COLOR_BLACK);
    }
    fb.fillRect(layout::ART_X + layout::ART_W / 2 - 40, layout::ART_Y + layout::ART_H / 2 - 30,
                80, 60, layout::COLOR_RED);
}