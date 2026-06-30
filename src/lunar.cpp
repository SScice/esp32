#include "lunar.h"

#include <Arduino.h>
#include <pgmspace.h>
#include <stdio.h>
#include <string.h>

struct LunarEntry {
    uint16_t solar_y;
    uint8_t solar_m;
    uint8_t solar_d;
    uint16_t lunar_y;
    uint8_t lunar_m;
    uint8_t lunar_d;
    uint8_t leap;
};

#include "lunar_data.inc"

struct LunarDay {
    int lunar_year;
    int lunar_month;
    int lunar_day;
    bool is_leap_month;
};

static const char kMonthNames[][9] = {
    u8"正月",
    u8"二月",
    u8"三月",
    u8"四月",
    u8"五月",
    u8"六月",
    u8"七月",
    u8"八月",
    u8"九月",
    u8"十月",
    u8"冬月",
    u8"腊月",
};

static const char kDigit[] = u8"一二三四五六七八九十";

static void appendUtf8(char* buf, size_t n, const char* s) {
    if (!buf || n == 0 || !s) return;
    snprintf(buf + strlen(buf), n - strlen(buf), "%s", s);
}

static bool lookupSolar(int y, int m, int d, LunarDay* out) {
    int lo = 0;
    int hi = (int)kLunarTableDays - 1;
    while (lo <= hi) {
        const int mid = (lo + hi) / 2;
        LunarEntry e;
        memcpy_P(&e, &kLunarTable[mid], sizeof(e));
        if (e.solar_y < y)
            lo = mid + 1;
        else if (e.solar_y > y)
            hi = mid - 1;
        else if (e.solar_m < m)
            lo = mid + 1;
        else if (e.solar_m > m)
            hi = mid - 1;
        else if (e.solar_d < d)
            lo = mid + 1;
        else if (e.solar_d > d)
            hi = mid - 1;
        else {
            out->lunar_year = e.lunar_y;
            out->lunar_month = e.lunar_m;
            out->lunar_day = e.lunar_d;
            out->is_leap_month = (e.leap != 0);
            return true;
        }
    }
    return false;
}

static bool lunarFromTm(const struct tm* tm, LunarDay* out) {
    if (!tm || !out) return false;
    const int y = tm->tm_year + 1900;
    const int m = tm->tm_mon + 1;
    const int d = tm->tm_mday;
    return lookupSolar(y, m, d, out);
}

static void formatLunarDaySuffix(char* buf, size_t n, int day) {
    if (n == 0) return;
    buf[0] = '\0';
    if (day < 1 || day > 30) return;

    if (day <= 10) {
        appendUtf8(buf, n, u8"初");
        if (day == 10)
            appendUtf8(buf, n, u8"十");
        else {
            char one[4] = {kDigit[day - 1], '\0'};
            appendUtf8(buf, n, one);
        }
        return;
    }
    if (day < 20) {
        appendUtf8(buf, n, u8"十");
        char one[4] = {kDigit[day - 11], '\0'};
        appendUtf8(buf, n, one);
        return;
    }
    if (day == 20) {
        appendUtf8(buf, n, u8"二十");
        return;
    }
    if (day < 30) {
        appendUtf8(buf, n, u8"廿");
        char one[4] = {kDigit[day - 21], '\0'};
        appendUtf8(buf, n, one);
        return;
    }
    appendUtf8(buf, n, u8"三十");
}

void formatLunarLine(char* buf, size_t n, const struct tm* tm) {
    if (!buf || n == 0) return;
    buf[0] = '\0';
    if (!tm) return;

    LunarDay ld;
    if (!lunarFromTm(tm, &ld)) return;

    if (ld.is_leap_month) appendUtf8(buf, n, u8"闰");

    if (ld.lunar_month >= 1 && ld.lunar_month <= 12)
        appendUtf8(buf, n, kMonthNames[ld.lunar_month - 1]);
    else
        return;

    appendUtf8(buf, n, u8"月");

    char daypart[16];
    formatLunarDaySuffix(daypart, sizeof(daypart), ld.lunar_day);
    appendUtf8(buf, n, daypart);
}