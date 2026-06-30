#include "clock_font.h"
#include "display_fb.h"
#include "layout_design.h"
#include <pgmspace.h>
#include <time.h>

#include "clock_font_data.inc"
#include "clock_font_lunar_data.inc"

struct StripGlyphEntry {
    int codepoint;
    const uint8_t* data;
};

static const StripGlyphEntry kStripGlyphs[] = {
    {'0', glyph_48}, {'1', glyph_49}, {'2', glyph_50}, {'3', glyph_51}, {'4', glyph_52},
    {'5', glyph_53}, {'6', glyph_54}, {'7', glyph_55}, {'8', glyph_56}, {'9', glyph_57},
    {':', glyph_58}, {0xFF08, glyph_65288}, {0xFF09, glyph_65289},
    {'U', glyph_85}, {'T', glyph_84}, {'C', glyph_67}, {'+', glyph_43}, {' ', glyph_32},
    {0x6B63, glyph_27491}, {0x4E8C, glyph_20108}, {0x4E09, glyph_19977}, {0x56DB, glyph_22235},
    {0x4E94, glyph_20116}, {0x516D, glyph_20845}, {0x4E03, glyph_19971}, {0x516B, glyph_20843},
    {0x4E5D, glyph_20061}, {0x5341, glyph_21313}, {0x814A, glyph_33098}, {0x95F0, glyph_38384},
    {0x6708, glyph_26376}, {0x521D, glyph_21021}, {0x5EFF, glyph_24319}, {0x5345, glyph_21317},
    {0x51AC, glyph_20908},
};

static int utf8CharLen(unsigned char c) {
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    return 1;
}

static int utf8Codepoint(const char* utf8, int len) {
    if (len == 1) return (unsigned char)utf8[0];
    if (len == 2)
        return ((utf8[0] & 0x1F) << 6) | (utf8[1] & 0x3F);
    if (len == 3)
        return ((utf8[0] & 0x0F) << 12) | ((utf8[1] & 0x3F) << 6) | (utf8[2] & 0x3F);
    return (unsigned char)utf8[0];
}

const uint8_t* clockGlyphForChar(const char* utf8, int* adv) {
    unsigned char c0 = (unsigned char)utf8[0];
    int len = utf8CharLen(c0);
    int cp = utf8Codepoint(utf8, len);
    for (const auto& g : kStripGlyphs) {
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

void drawStripString(Framebuffer& fb, int x, int y, const char* s) {
    const int row_bytes = (CLOCK_GLYPH_W + 7) / 8;
    uint8_t line[row_bytes * CLOCK_GLYPH_H];

    while (s && *s) {
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

void drawClockString(Framebuffer& fb, int cx, int cy, const char* s) {
    const int total = clockStringWidth(s);
    const int x = cx - total / 2;
    const int y = cy - CLOCK_GLYPH_H / 2;
    drawStripString(fb, x, y, s);
}

void formatClockLine(char* buf, size_t buflen, const struct tm* t) {
    snprintf(buf, buflen, "%02d:%02d%s", t->tm_hour, t->tm_min, layout::CLOCK_TZ_SUFFIX);
}