#pragma once
#include <Arduino.h>
#include <pgmspace.h>

#define CLOCK_GLYPH_W 20
#define CLOCK_GLYPH_H 28
#define CLOCK_GLYPH_BYTES ((CLOCK_GLYPH_W + 7) / 8 * CLOCK_GLYPH_H)

/* Clock line (drawClockString): ASCII 0-9, ':', 'U','T','C','+',' ',
 * fullwidth parens U+FF08 U+FF09. Data in src/clock_font_data.inc.
 *
 * Lunar line (drawLunarString): PROGMEM 20x28 Han in src/clock_font_lunar_data.inc:
 *   正 二 三 四 五 六 七 八 九 十 腊 闰 月 初 廿 卅 冬
 * UTF-8: each character is 3 bytes (E4–E9 / E5 / E6 / E8 / E9 leading byte).
 * Unknown codepoints render as blank (half-width advance).
 */

const uint8_t* clockGlyphForChar(const char* utf8, int* adv);

int clockStringWidth(const char* s);
void drawClockString(class Framebuffer& fb, int cx, int cy, const char* s);
void formatClockLine(char* buf, size_t buflen, const struct tm* t);

const uint8_t* lunarGlyphForChar(const char* utf8, int* adv);
int lunarStringWidth(const char* s);
void drawLunarString(class Framebuffer& fb, int x, int y, const char* utf8);