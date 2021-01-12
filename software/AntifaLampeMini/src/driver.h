#pragma once

#include <Arduino.h>
#include <stdint.h>

void driver_init();
void set_brightness(uint8_t val);
void set_normalize(bool val);
void show_leds();

void set_led_rgb(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
void set_led_hsv(uint8_t idx, uint8_t h, uint8_t s, uint8_t v);

void set_segment_rgb(uint8_t idx0, uint8_t idx1, uint8_t r, uint8_t g, uint8_t b);
void set_segment_hsv(uint8_t idx0, uint8_t idx1, uint8_t h, uint8_t s, uint8_t v);

void set_xy_rgb(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
void set_xy_hsv(uint8_t x, uint8_t y, uint8_t h, uint8_t s, uint8_t v);