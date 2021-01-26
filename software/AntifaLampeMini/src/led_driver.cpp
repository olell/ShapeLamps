#include <FastLED.h>
#include <Arduino.h>
#include <stdint.h>

#include "led_driver.h"
#include "config.h"
#include "util.h"

CRGB leds[NUM_LEDS];

uint8_t brightness = BOOT_BRIGHTNESS;
bool normalize = false;

void driver_init() {
    FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS); //.setCorrection(TypicalSMD5050);
    FastLED.setBrightness(255);
}

void set_brightness(uint8_t val) {
    brightness = val;
    if (brightness > MAX_BRIGHTNESS)
        brightness = MAX_BRIGHTNESS;
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

CRGB color_brightness(CRGB color) {
    color.r *= (brightness / 255.0);
    color.g *= (brightness / 255.0);
    color.b *= (brightness / 255.0);
    return color;
}

void set_led_rgb(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    if (!normalize)
        leds[idx] = color_brightness(CRGB(r, g, b));
    else
        leds[idx] = normalize_color(CRGB(r, g, b));
}

void add_led_rgb(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    CRGB current_color = leds[idx];
    CRGB add_color;
    if (!normalize)
        add_color = color_brightness(CRGB(r, g, b));
    else
        add_color = normalize_color(CRGB(r, g, b));

    current_color.r += add_color.r;
    current_color.g += add_color.g;
    current_color.b += add_color.b;
    
    leds[idx] = current_color;
}

void set_led_hsv(uint8_t idx, uint8_t h, uint8_t s, uint8_t v) {
    CRGB rgb_col;
    hsv2rgb_rainbow(CHSV(h, s, v), rgb_col);
    if (!normalize)
        leds[idx] = color_brightness(rgb_col);
    else
        leds[idx] = normalize_color(rgb_col);
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
            set_led_hsv(idx, h, s, v);
    }
}

void display_ring_percent(float percent, uint8_t r, uint8_t g, uint8_t b) {
    set_segment_rgb(0, NUM_LEDS, 0, 0, 0);
    float led_count = (RING_LAST_LED - RING_FIRST_LED) * percent;
    float last_led = led_count - ((int) led_count);
    for (int i = 0; i < led_count; i ++) {
        set_led_rgb(RING_FIRST_LED + i, r, g, b);
    }
    if (led_count < (RING_LAST_LED - RING_FIRST_LED)) {
        set_led_rgb(RING_FIRST_LED + led_count, r * last_led, g * last_led, b * last_led);
    }
    show_leds();
}

float current_cycle_pos = 0;

void do_cycle(float speed) {

    set_segment_rgb(0, NUM_LEDS, 0, 0, 0);
    set_led_rgb(RING_FIRST_LED + (int)current_cycle_pos, 255, 255, 255);

    show_leds();

    current_cycle_pos += speed;
    if (current_cycle_pos > (RING_LAST_LED - RING_FIRST_LED)) current_cycle_pos = 0;
}

void show_leds() {
    FastLED.show();
}

float get_current_power_consumption() {

    uint16_t pred = 0;
    uint16_t pgreen = 0;
    uint16_t pblue = 0;
    for (int i = 0; i < NUM_LEDS; i++) {
        pred += leds[i].r;
        pgreen += leds[i].g;
        pblue += leds[i].b;
    }
    float red_power = (pred / (NUM_LEDS * 255.0)) * (NUM_LEDS * 20); // mA
    float green_power = (pgreen / (NUM_LEDS * 255.0)) * (NUM_LEDS * 20); // mA
    float blue_power = (pblue / (NUM_LEDS * 255.0)) * (NUM_LEDS * 20); // mA

    return red_power + green_power + blue_power + 300; // 250mA power for ESP and stuff
    
}