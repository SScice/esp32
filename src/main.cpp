#include <Arduino.h>
#include <time.h>
#include "display_compositor.h"
#include "wifi_ntp.h"
#include "layout_design.h"

static DisplayCompositor g_display;
static int g_last_yday = -1;
static int g_last_min = -1;
static struct tm g_time_cache;

static bool readTime(struct tm* out) {
    time_t now = time(nullptr);
    if (now < 1700000000) return false;
    localtime_r(&now, out);
    return true;
}

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println(F("epd7in5bc calendar clock"));

    if (!g_display.allocate()) {
        Serial.println(F("framebuffer alloc failed"));
        while (true) delay(1000);
    }

    wifiNtpBegin();
    if (!wifiNtpSync(&g_time_cache, 25000)) {
        Serial.println(F("WiFi/NTP pending — using placeholder time"));
        g_time_cache = {};
        g_time_cache.tm_year = 126;
        g_time_cache.tm_mon = 2;
        g_time_cache.tm_mday = 30;
        g_time_cache.tm_hour = 12;
        g_time_cache.tm_min = 0;
    }

    if (g_display.epd.Init() != 0) {
        Serial.println(F("EPD init failed"));
    }

    g_display.composeFull(&g_time_cache);
    g_display.pushFrame();
    g_last_yday = g_time_cache.tm_yday;
    g_last_min = g_time_cache.tm_hour * 60 + g_time_cache.tm_min;
}

void loop() {
    wifiNtpMaintain(6);

    struct tm now;
    if (!readTime(&now)) return;

    const int minute_key = now.tm_hour * 60 + now.tm_min;
    const bool date_changed = (now.tm_yday != g_last_yday) || (g_last_yday < 0);
    const bool minute_changed = (minute_key != g_last_min);

    if (date_changed) {
        g_time_cache = now;
        g_display.composeFull(&g_time_cache);
        g_display.pushFrame();
        g_last_yday = now.tm_yday;
        g_last_min = minute_key;
        g_display.epd.Sleep();
        delay(50);
        g_display.epd.Init();
        return;
    }

    if (minute_changed) {
        g_time_cache = now;
        g_display.drawBottomStrip(&g_time_cache);
        g_display.pushFrame();
        g_last_min = minute_key;
        return;
    }

    delay(2000);
}