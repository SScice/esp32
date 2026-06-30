#pragma once
// User-replaceable 256×307 tri-color bitmap (MSB-first 1bpp per plane).
// Replace blitCustomImage() implementation or asset arrays in custom_image.cpp.

#include <Arduino.h>

void blitCustomImage(class Framebuffer& fb);