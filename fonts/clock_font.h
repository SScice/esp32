#pragma once
#include <Arduino.h>
#include <pgmspace.h>

#define CLOCK_GLYPH_W 20
#define CLOCK_GLYPH_H 28
#define CLOCK_GLYPH_BYTES ((CLOCK_GLYPH_W + 7) / 8 * CLOCK_GLYPH_H)

/* Bottom-strip glyphs (drawStripString / drawClockString): ASCII clock chars in
 * src/clock_font_data.inc; Han lunar chars in src/clock_font_lunar_data.inc:
 *   正 二 三 四 五 六 七 八 九 十 腊 闰 月 初 廿 卅 冬
 * Unknown codepoints render as blank (half-width advance).
 */

const uint8_t* clockGlyphForChar(const char* utf8, int* adv);

int clockStringWidth(const char* s);
void drawStripString(class Framebuffer& fb, int x, int y, const char* s);
void drawClockString(class Framebuffer& fb, int cx, int cy, const char* s);
void formatClockLine(char* buf, size_t buflen, const struct tm* t);