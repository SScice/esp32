#pragma once
#include "display_fb.h"
#include "epd7in5b.h"
#include <time.h>

class DisplayCompositor {
public:
    uint8_t* black;
    uint8_t* red;
    Framebuffer fb;
    Epd epd;

    DisplayCompositor();
    ~DisplayCompositor();
    bool allocate();
    void composeFull(const struct tm* now);
    void drawBottomStrip(const struct tm* now);
    void pushFrame();
};