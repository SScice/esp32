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

// Bottom strip: lunar (left) + clock (center). Parity: design/calendar-display.html footer.
constexpr int BOTTOM_LUNAR_X = 16;  // left margin; matches calendar panel inset (CAL_X + 8).
// Clock anchor: horizontal center of full 640px strip (drawClockString uses center x).
constexpr int BOTTOM_CLOCK_CENTER_X = PANEL_W / 2;
// Vertical center of strip content; +2 nudges baseline with clock glyph metrics.
constexpr int BOTTOM_CLOCK_CENTER_Y = BOTTOM_Y + BOTTOM_H / 2 + 2;

// snprintf buffer for bottom strip strings: lunar line + clock line (formatClockLine).
// Lunar UTF-8 can be ~24–36 bytes; clock "HH:MM（UTC+8）" ≈ 22 bytes — use ≥48, prefer 64.
constexpr int BOTTOM_STRIP_LINE_BUF = 64;

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