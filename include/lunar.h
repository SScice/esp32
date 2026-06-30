#pragma once

#include <stddef.h>
#include <time.h>

/**
 * Format lunar date as UTF-8, e.g. "正月初一", "闰腊月初三十".
 * On lookup failure writes empty string.
 */
void formatLunarLine(char* buf, size_t n, const struct tm* tm);