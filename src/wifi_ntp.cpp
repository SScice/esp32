#include "wifi_ntp.h"
#include <ESP8266WiFi.h>

#if __has_include("secrets.h")
#include "secrets.h"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "your-ssid"
#endif
#ifndef WIFI_PASS
#define WIFI_PASS "your-password"
#endif

static unsigned long s_last_sync_ms = 0;
static const long GMT_OFFSET_SEC = 8 * 3600;
static const int DAYLIGHT_OFFSET_SEC = 0;

void wifiNtpBegin() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

bool wifiNtpSync(struct tm* out, unsigned timeout_ms) {
    const unsigned start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < timeout_ms) {
        delay(250);
    }
    if (WiFi.status() != WL_CONNECTED) return false;

    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, "pool.ntp.org", "ntp.aliyun.com");
    for (int i = 0; i < 20; i++) {
        time_t now = time(nullptr);
        if (now > 1700000000) {
            localtime_r(&now, out);
            s_last_sync_ms = millis();
            return true;
        }
        delay(500);
    }
    return false;
}

void wifiNtpMaintain(unsigned resync_hours) {
    if (WiFi.status() != WL_CONNECTED) {
        wifiNtpBegin();
        return;
    }
    const unsigned long period = (unsigned long)resync_hours * 3600000UL;
    if (s_last_sync_ms && millis() - s_last_sync_ms < period) return;
    struct tm t;
    wifiNtpSync(&t, 15000);
}