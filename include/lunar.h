#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <time.h>

/** Lunar date on the China civil calendar day represented by *tm. */
struct LunarDay {
    int lunar_year;   /**< e.g. 2024 */
    int lunar_month;  /**< 1..12 */
    int lunar_day;    /**< 1..30 */
    bool is_leap_month;
};

/**
 * Map a broken-down local civil date to lunar (China GB/T 33661-style table).
 *
 * @param tm  Local time from localtime_r after configTime(UTC+8) — tm_year,
 *            tm_mon, tm_mday are used; tm is not modified.
 * @param out Filled on success.
 * @return true if tm falls within the compiled lookup range (2020–2035).
 */
bool lunar_day_from_tm(const struct tm* tm, LunarDay* out);

/**
 * Format lunar date as UTF-8, e.g. "正月初一", "闰腊月初三十".
 * On lookup failure writes empty string.
 */
void formatLunarLine(char* buf, size_t n, const struct tm* tm);