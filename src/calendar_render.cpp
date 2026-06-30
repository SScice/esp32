#include "calendar_render.h"
#include "layout_design.h"
#include <Arduino.h>
#include <stdio.h>

static const char* kMonthZh[] = {
    "一月", "二月", "三月", "四月", "五月", "六月",
    "七月", "八月", "九月", "十月", "十一月", "十二月"
};

static int daysInMonth(int y, int m) {
    static const int dim[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int d = dim[m - 1];
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))) d++;
    return d;
}

static int weekdaySun0(int y, int m, int d) {
    struct tm t = {};
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    mktime(&t);
    return t.tm_wday;
}

static void drawTextBlock(Framebuffer& fb, int x, int y, const char* text, int scale, uint8_t color) {
    int cx = x;
    while (*text) {
        unsigned char c = (unsigned char)*text++;
        if (c < 32) continue;
        for (int sy = 0; sy < 7 * scale; sy++) {
            for (int sx = 0; sx < 5 * scale; sx++) {
                bool on = ((c + sx + sy) % 3 == 0);
                if (on) fb.setPixel(cx + sx, y + sy, color);
            }
        }
        cx += 6 * scale;
    }
}

void drawCalendarPanel(Framebuffer& fb, const struct tm* now) {
    const int ox = layout::CAL_X;
    const int oy = layout::CAL_Y;
    fb.fillRect(ox, oy, layout::CAL_W, layout::CAL_H, layout::COLOR_PAPER);

    char header[48];
    snprintf(header, sizeof(header), "%d年 %s", now->tm_year + 1900, kMonthZh[now->tm_mon]);
    drawTextBlock(fb, ox + 8, oy + 6, header, 1, layout::COLOR_BLACK);

    const int grid_x = ox + 6;
    const int grid_y = oy + 28;
    const int cell_w = (layout::CAL_W - 12) / 7;
    const int cell_h = (layout::CAL_H - 36) / 7;

    for (int i = 0; i < 7; i++) {
        int wx = grid_x + i * cell_w + cell_w / 2 - 4;
        uint8_t col = (i == 0) ? layout::COLOR_RED : layout::COLOR_BLACK;
        for (int dy = 0; dy < 8; dy++)
            for (int dx = 0; dx < 8; dx++)
                if ((layout::WEEKDAY_LABELS[i][0] + dx + dy) % 2 == 0)
                    fb.setPixel(wx + dx, grid_y + dy, col);
    }

    fb.drawHLine(grid_x, grid_y + 14, layout::CAL_W - 12, layout::COLOR_BLACK);

    const int y = now->tm_year + 1900;
    const int m = now->tm_mon + 1;
    const int today = now->tm_mday;
    const int first_wd = weekdaySun0(y, m, 1);
    const int dim = daysInMonth(y, m);

    int day = 1;
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            const int cell = row * 7 + col;
            if (cell < first_wd || day > dim) continue;
            const int cx = grid_x + col * cell_w + cell_w / 2;
            const int cy = grid_y + 18 + row * cell_h + cell_h / 2;
            const bool is_today = (day == today);
            const bool sunday = (col == 0);
            if (is_today)
                fb.fillRect(grid_x + col * cell_w + 2, grid_y + 16 + row * cell_h + 2,
                            cell_w - 4, cell_h - 4, layout::COLOR_RED);
            const uint8_t color = is_today ? layout::COLOR_PAPER : (sunday ? layout::COLOR_RED : layout::COLOR_BLACK);
            char num[4];
            snprintf(num, sizeof(num), "%d", day);
            drawTextBlock(fb, cx - 4, cy - 4, num, 1, color);
            day++;
        }
    }
}