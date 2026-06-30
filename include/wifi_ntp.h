#pragma once
#include <Arduino.h>
#include <time.h>

void wifiNtpBegin();
bool wifiNtpSync(struct tm* out, unsigned timeout_ms = 20000);
void wifiNtpMaintain(unsigned resync_hours = 6);