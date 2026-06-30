#pragma once
#include <Arduino.h>
#include <pgmspace.h>

#define CLOCK_GLYPH_W 20
#define CLOCK_GLYPH_H 28
#define CLOCK_GLYPH_BYTES ((CLOCK_GLYPH_W + 7) / 8 * CLOCK_GLYPH_H)

const uint8_t* clockGlyphForChar(const char* utf8, int* adv);

int clockStringWidth(const char* s);
void drawClockString(class Framebuffer& fb, int cx, int cy, const char* s);
void formatClockLine(char* buf, size_t buflen, const struct tm* t);