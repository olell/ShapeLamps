#include <FastLED.h>
#include <Arduino.h>
#include <stdint.h>

#include "driver.h"
#include "config.h"

CRGB leds[NUM_LEDS];

uint8_t brightness = BOOT_BRIGHTNESS;
bool normalize = false;

void driver_init() {
    FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
    FastLED.setBrightness(brightness);
}

void set_brightness(uint8_t val) {
    brightness = val;
    if (brightness > MAX_BRIGHTNESS)
        brightness = MAX_BRIGHTNESS;
    FastLED.setBrightness(brightness);
}

void set_normalize(bool val) {
    normalize = val;
}

CRGB normalize_color(CRGB color) {
    uint16_t rgbsum = color.r + color.g + color.b;
    uint16_t normbright = brightness * 3;
    if (rgbsum > normbright) {
        float divider = (float) rgbsum / normbright;
        if (divider > 0) {
            color.r /= divider;
            color.g /= divider;
            color.b /= divider;
        }
        if (color.r > 255) color.r = 255;
        if (color.g > 255) color.g = 255;
        if (color.b > 255) color.b = 255;
    }
    return color;
}

void set_led_rgb(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    if (!normalize)
        leds[idx] = CRGB(r, g, b);
    else
        leds[idx] = normalize_color(CRGB(r, g, b));
}

void set_led_hsv(uint8_t idx, uint8_t h, uint8_t s, uint8_t v) {
    if (!normalize)
        leds[idx] = CRGB(CHSV(h, s, v));
    else
        leds[idx] = normalize_color(CRGB(CHSV(h, s, v)));
}

void set_segment_rgb(uint8_t idx0, uint8_t idx1, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = idx0; i < idx1; i ++)
        set_led_rgb(i, r, g, b);
}

void set_segment_hsv(uint8_t idx0, uint8_t idx1, uint8_t h, uint8_t s, uint8_t v) {
    for (int i = idx0; i < idx1; i ++)
        set_led_hsv(i, h, s, v);
}

void set_xy_rgb(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b){
    uint8_t idx;
    for (int i = 0; i < LED_GRID_PER_CELL; i ++) {
        idx = led_grid[y][x][i];
        if (idx >= 0)
            set_led_rgb(idx, r, g, b);
    }
}

void set_xy_hsv(uint8_t x, uint8_t y, uint8_t h, uint8_t s, uint8_t v){
    uint8_t idx;
    for (int i = 0; i < LED_GRID_PER_CELL; i ++) {
        idx = led_grid[y][x][i];
        if (idx >= 0)
            set_led_rgb(idx, h, s, v);
    }
}

void show_leds() {
    FastLED.show();
}