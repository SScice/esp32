#include "clock_font.h"
#include "display_fb.h"
#include "layout_design.h"
#include <pgmspace.h>
#include <time.h>

#include "clock_font_data.inc"

struct ClockGlyphEntry {
    int codepoint;
    const uint8_t* data;
};

static const ClockGlyphEntry kClockGlyphs[] = {
    {'0', glyph_48}, {'1', glyph_49}, {'2', glyph_50}, {'3', glyph_51}, {'4', glyph_52},
    {'5', glyph_53}, {'6', glyph_54}, {'7', glyph_55}, {'8', glyph_56}, {'9', glyph_57},
    {':', glyph_58}, {0xFF08, glyph_65288}, {0xFF09, glyph_65289},
    {'U', glyph_85}, {'T', glyph_84}, {'C', glyph_67}, {'+', glyph_43}, {' ', glyph_32},
};

static int utf8CharLen(unsigned char c) {
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    return 1;
}

const uint8_t* clockGlyphForChar(const char* utf8, int* adv) {
    unsigned char c0 = (unsigned char)utf8[0];
    int len = utf8CharLen(c0);
    int cp = c0;
    if (len == 3)
        cp = ((utf8[0] & 0x0F) << 12) | ((utf8[1] & 0x3F) << 6) | (utf8[2] & 0x3F);
    for (const auto& g : kClockGlyphs) {
        if (g.codepoint == cp) {
            if (adv) *adv = CLOCK_GLYPH_W;
            return g.data;
        }
    }
    if (adv) *adv = CLOCK_GLYPH_W / 2;
    return glyph_32;
}

int clockStringWidth(const char* s) {
    int w = 0;
    while (*s) {
        int adv = 0;
        clockGlyphForChar(s, &adv);
        int len = utf8CharLen((unsigned char)*s);
        w += adv;
        s += len;
    }
    return w;
}

void drawClockString(Framebuffer& fb, int cx, int cy, const char* s) {
    const int total = clockStringWidth(s);
    int x = cx - total / 2;
    const int y = cy - CLOCK_GLYPH_H / 2;
    const int row_bytes = (CLOCK_GLYPH_W + 7) / 8;
    uint8_t line[row_bytes * CLOCK_GLYPH_H];

    while (*s) {
        int adv = 0;
        const uint8_t* gd = clockGlyphForChar(s, &adv);
        int len = utf8CharLen((unsigned char)*s);
        for (int i = 0; i < CLOCK_GLYPH_BYTES; i++)
            line[i] = pgm_read_byte(gd + i);
        fb.blit1bpp(x, y, CLOCK_GLYPH_W, CLOCK_GLYPH_H, line, layout::COLOR_BLACK, layout::COLOR_PAPER);
        x += adv;
        s += len;
    }
}

void formatClockLine(char* buf, size_t buflen, const struct tm* t) {
    snprintf(buf, buflen, "%02d:%02d%s", t->tm_hour, t->tm_min, layout::CLOCK_TZ_SUFFIX);
}