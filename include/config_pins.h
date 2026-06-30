#pragma once

// E-paper GPIO — change to match your wiring (ESP8266 Dn labels = GPIO numbers below).
// Waveshare 7.5" B/C HAT often uses: BUSY D2, RST D4, DC D3, CS D8, CLK D5, MOSI D7.

#ifndef EPD_RST_PIN
#define EPD_RST_PIN 2   // D4
#endif
#ifndef EPD_DC_PIN
#define EPD_DC_PIN 0    // D3
#endif
#ifndef EPD_CS_PIN
#define EPD_CS_PIN 15   // D8
#endif
#ifndef EPD_BUSY_PIN
#define EPD_BUSY_PIN 4  // D2
#endif
#ifndef EPD_PWR_PIN
#define EPD_PWR_PIN (-1)  // optional panel power GPIO, or -1 to skip
#endif

#define RST_PIN EPD_RST_PIN
#define DC_PIN EPD_DC_PIN
#define CS_PIN EPD_CS_PIN
#define BUSY_PIN EPD_BUSY_PIN
#define PWR_PIN EPD_PWR_PIN