#pragma once
// Layout parity with design/calendar-display.html and PRD (640×384 epd7in5bc).

#include <cstdint>

namespace layout {

constexpr int PANEL_W = 640;
constexpr int PANEL_H = 384;

constexpr int BOTTOM_H = 77;

constexpr int CAL_X = 0;
constexpr int CAL_Y = 0;
constexpr int CAL_W = 384;
constexpr int CAL_H = 307;

constexpr int ART_X = 384;
constexpr int ART_Y = 0;
constexpr int ART_W = 256;
constexpr int ART_H = 307;

constexpr int BOTTOM_Y = 307;
constexpr int BOTTOM_STRIP_BORDER_PX = 2;

// design/:root --epd-paper / --epd-black / --epd-red
constexpr uint8_t COLOR_PAPER = 1;   // white / paper
constexpr uint8_t COLOR_BLACK = 0;
constexpr uint8_t COLOR_RED = 2;

constexpr int PLANE_BYTES = (PANEL_W / 8) * PANEL_H;

// Sunday-first weekday labels (UTF-8 for UI strings; rendering uses glyph module)
constexpr const char* WEEKDAY_LABELS[7] = {
    "日", "一", "二", "三", "四", "五", "六"
};

// Clock display: xx:yy（UTC+8） — fullwidth parens per PRD
constexpr const char* CLOCK_TZ_SUFFIX = "（UTC+8）";

}  // namespace layout