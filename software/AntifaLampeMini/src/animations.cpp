#include <stdint.h>
#include <Arduino.h>
#include <math.h>

#include "config.h"
#include "animations.h"
#include "led_driver.h"
#include "util.h"

const String anim_off_name = "off";
const String anim_rainbow_name = "rainbow";
const String anim_rainbow_cycle_name = "rainbow_cycle";
const String anim_rainbow_bars_name = "rainbow_bars";
const String anim_single_color_name = "single_color";
const String anim_clock_name = "clock";
const String anim_fire_name = "fire";
const String anim_matrix_name = "matrix";

uint8_t running_animation = OFF_ANIMATION;

float animation_speed = 127;

// rainbow animation
int16_t rainbow_anim_hue = 0;

// clock animation
struct tm timeinfo;

// single color animation
uint8_t sc_pr, sc_pg, sc_pb; // previous colors
uint8_t sc_r, sc_g, sc_b;
float sc_fadeover = 0;

// fire animation ... maybe a bit memory consuming
float matrix_prev[LED_GRID_HEIGHT][LED_GRID_WIDTH];
float matrix[LED_GRID_HEIGHT][LED_GRID_WIDTH];


void rainbow_animation() {
    set_segment_hsv(0, NUM_LEDS, (rainbow_anim_hue / 1000.0) * 255, 255, 255);
    rainbow_anim_hue += (animation_speed / 255.0) * 8;
    if (rainbow_anim_hue > 1000) rainbow_anim_hue = 0;
}

void rainbow_cycle_animation() {
    float hue;
    for (int i = RING_FIRST_LED; i <= RING_LAST_LED; i ++) {
        hue = ((float)rainbow_anim_hue / 1000.0) + ((float) (i - RING_FIRST_LED) / float(RING_LAST_LED - RING_FIRST_LED));
        if (hue >= 1) hue -= 1;

        set_led_hsv(i, hue * 255, 255, 255);

        if (i == 46) {
            set_segment_hsv(SMALL_FLAG_FIRST_LED, SMALL_FLAG_LAST_LED + 1, hue * 255, 255, 255);
        }
        if (i == 75) {
            set_segment_hsv(BIG_FLAG_FIRST_LED, BIG_FLAG_LAST_LED + 1, hue * 255, 255, 255);
        }
    }
    rainbow_anim_hue += (animation_speed / 255.0) * 8;
    if (rainbow_anim_hue > 1000) rainbow_anim_hue = 0;
    
}

void single_color_animation() {
    sc_fadeover += 0.001 + (animation_speed / 2560.0);
    if (sc_fadeover > 1)
            sc_fadeover = 1;

    uint8_t r, g, b;
    r = (sc_r * sc_fadeover) + (sc_pr * (1 - sc_fadeover));
    g = (sc_g * sc_fadeover) + (sc_pg * (1 - sc_fadeover));
    b = (sc_b * sc_fadeover) + (sc_pb * (1 - sc_fadeover));
    set_segment_rgb(0, NUM_LEDS, r, g, b);
}

void rainbow_bars_animation() {
    uint8_t hue;
    for (int y = 0; y < LED_GRID_HEIGHT; y ++) {
        hue = ((float) y / LED_GRID_HEIGHT) * 255;
        hue += (rainbow_anim_hue / 1000.0) * 255;
        for (int x = 0; x < LED_GRID_WIDTH; x ++) {
            set_xy_hsv(x, y, hue, 255, 255);
        }
    }
    rainbow_anim_hue += (animation_speed / 255.0) * 8;
    if (rainbow_anim_hue > 1000) rainbow_anim_hue = 0;
}

void clock_animation() {
    struct tm now = get_time();
    int ring_top_led = 67;
    int ring_first_led = RING_FIRST_LED;
    int ring_last_led = RING_LAST_LED;
    int ring_count = RING_LAST_LED - RING_FIRST_LED;

    int hours = 12;

    int hour = now.tm_hour % hours; // 12 hour wraparound
    int minute = now.tm_min;
    int second = now.tm_sec;

    int hour_led = ring_top_led + ((float) hour / 12.0) * ring_count;
    if (hour_led > ring_last_led) hour_led = (hour_led - ring_last_led) + ring_first_led;
    
    float min_f = ((float) minute / 60.0);
    int minute_led = ring_top_led + min_f * ring_count;
    if (minute_led > ring_last_led) minute_led = (minute_led - ring_last_led) + ring_first_led;
    
    float next_min_f = ((float) ((minute + 1) % 60) / 60.0);
    int minute_next_led = ring_top_led + next_min_f * ring_count;
    if (minute_next_led > ring_last_led) minute_next_led = (minute_next_led - ring_last_led) + ring_first_led;
    
    float sec_f = ((float) second / 60.0);

    set_segment_rgb(0, NUM_LEDS, 0, 0, 0); // clear

    add_led_rgb(hour_led, 255, 0, 0);  
    add_led_rgb(minute_led, 0, 0, 255 * sec_f);
    add_led_rgb(minute_next_led, 0, 0, 255 * (1-sec_f));

}

void free_matrix() {
    for (int y = 0; y < LED_GRID_HEIGHT; y ++) {
        for (int x = 0; x < LED_GRID_WIDTH; x++) {
            matrix[y][x] = 0;
        }
    }
}

void fire_animation () {

    int x, y;
    float heat;

    float min_cooldown = 0.01;
    float max_cooldown = 0.15;

    // 1. cooldown
    for (y = 0; y < LED_GRID_HEIGHT; y ++) {
        for (x = 0; x < LED_GRID_WIDTH; x ++) {
            heat = matrix[y][x];
            heat -= min_cooldown + random_float() * (max_cooldown - min_cooldown);
            matrix[y][x] = MAX(0, heat);
        }
    }

    // 2. raise
    float spreadsum = 0;
    for (y = LED_GRID_HEIGHT - 1; y >= 0; y --) {
        for (x = 0; x < LED_GRID_WIDTH; x ++) {
            heat = matrix[y][x] * 2;
            spreadsum = 2;
            if (y >= 1) {
                heat += matrix[y - 1][x] * 4;
                spreadsum += 4;
            }
            if (y >= 2) {
                heat += matrix[y - 2][x] * 3;
                spreadsum += 3;
            }
            if (y >= 3) {
                heat += matrix[y - 3][x] * 2;
                spreadsum += 2;
            }
            heat /= spreadsum;
            matrix[y][x] = heat;
        }
    }

    // 3. ignite
    for (y = 0; y < 4; y ++) {
        for (x = 0; x < LED_GRID_WIDTH; x ++) {
            if (random(0, 100) < 45) {
                matrix[y][x] += random_float() * 1.4;
                while (matrix[y][x] > 1) matrix[y][x] -= 1;
            }
        }
    }

    // "Lowpass"TM
    float prev_heat = 0;
    float lp_factor = 1;
    for (y = 0; y < LED_GRID_HEIGHT; y++) {
        for (x = 0; x < LED_GRID_WIDTH; x++) {
            heat = matrix[y][x];
            prev_heat = matrix_prev[y][x];
            lp_factor = animation_speed / 255.0;
            matrix_prev[y][x] = (1-lp_factor) * prev_heat + lp_factor * heat;
        }
    }

    // 4. display
    int h, s, v;
    for (y = 0; y < LED_GRID_HEIGHT; y ++) {
        for (x = 0; x < LED_GRID_WIDTH; x ++) {
            heat = matrix_prev[y][x];
            h = heat * 40; // Hue from read to yellowish
            s = (-heat + 0.98) * 1536;
            s = MAX(0, MIN(255, s));
            v = heat * 1024;
            v = MAX(0, MIN(255, v));
            set_xy_hsv(x, y, h, s, v);
        }
    }
}

void matrix_animation () {
    int x, y;

    float speed_factor = (animation_speed / 255.0) * 0.4;

    // Fall
    for (y = LED_GRID_HEIGHT - 2; y >= 0; y --) {
        for (x = 0; x < LED_GRID_WIDTH; x ++) {
            matrix[y][x] += matrix[y+1][x] * speed_factor;
            matrix[y+1][x] *= (1.0 - speed_factor);
        }
    }

    // New drops
    for (x = 0; x < LED_GRID_WIDTH; x ++) {
        if (random(0, 1000) < 15) {
            matrix[LED_GRID_HEIGHT - 1][x] += random_float();
            if (matrix[LED_GRID_HEIGHT - 1][x] > 1) matrix[LED_GRID_HEIGHT - 1][x] = 1;
        }
    }

    // Display
    int h, s, v;
    for (y = 0; y < LED_GRID_HEIGHT; y ++) {
        for (x = 0; x < LED_GRID_WIDTH; x ++) {
            h = 85;
            s = 255;
            v = 180 + (75 * matrix[y][x]);
            if (matrix[y][x] < 0.1) v = 0;
            set_xy_hsv(x, y, h, s, v);
        }
    }

}

void random_fade_animation () {

    float h, s, v;
    float p = (rainbow_anim_hue / 1000.0);
    h = (sc_pr * (1-p)) + (sc_r * p);
    s = (sc_pg * (1-p)) + (sc_g * p);
    v = (sc_pb * (1-p)) + (sc_b * p);

    set_segment_hsv(0, NUM_LEDS, h, s, v);

    rainbow_anim_hue += (animation_speed / 255.) * 16;
    if (rainbow_anim_hue > 1000) {
        rainbow_anim_hue = 0;
        sc_pr = sc_r;
        sc_pg = sc_g;
        sc_pb = sc_b;
        sc_r = random(255);
        sc_g = 100 + random(155);
        sc_b = 100 + random(155);

    }
}

void set_single_color(uint8_t r, uint8_t g, uint8_t b) {
    sc_fadeover = 0;
    sc_pr = sc_r;
    sc_pg = sc_g;
    sc_pb = sc_b;
    sc_r = r;
    sc_g = g;
    sc_b = b;
    if (running_animation != SINGLE_COLOR) {
        start_animation(anim_single_color_name);
        sc_fadeover = 1; // don't fade
    }
}

void dev_animation() {}

void start_animation(String name) {
    log_info("Selected animation \"%s\"", name.c_str());
    set_segment_rgb(0, NUM_LEDS, 0, 0, 0); // Clear
    enable_frame_limit(); // If prev animation disabled it
    free_matrix();
    if (name == anim_off_name) {
        running_animation = OFF_ANIMATION;
    }
    else if (name == anim_rainbow_name) {
        rainbow_anim_hue = 0;
        running_animation = RAINBOW_ANIMATION;
    }
    else if (name == anim_rainbow_cycle_name) {
        rainbow_anim_hue = 0;
        running_animation = RAINBOW_CYCLE_ANIMATION;
    }
    else if (name == anim_single_color_name) {
        running_animation = SINGLE_COLOR;
    }
    else if (name == anim_rainbow_bars_name) {
        rainbow_anim_hue = 0;
        running_animation = RAINBOW_BARS;
    }
    else if (name == anim_clock_name) {
        running_animation = CLOCK;
    }
    else if (name == anim_fire_name) {
        running_animation = FIRE_ANIMATION;
    }
    else if (name == anim_matrix_name) {
        running_animation = MATRIX_ANIMATION;
    }
    else if (name == "dev") running_animation = 100;
}

void run_animations() {
    if (running_animation == OFF_ANIMATION) {
        set_segment_rgb(0, NUM_LEDS, 0, 0, 0); // Clear
    }

    if (running_animation == RAINBOW_ANIMATION) {
        rainbow_animation();
    }

    if (running_animation == RAINBOW_CYCLE_ANIMATION) {
        rainbow_cycle_animation();
    }

    if (running_animation == SINGLE_COLOR) {
        single_color_animation();
    }

    if (running_animation == RAINBOW_BARS) {
        rainbow_bars_animation();
    }

    if (running_animation == CLOCK) {
        clock_animation();
    }

    if (running_animation == FIRE_ANIMATION) {
        fire_animation();
    }

    if (running_animation == MATRIX_ANIMATION) {
        matrix_animation();
    }

    if (running_animation == 100) {
        dev_animation();
    }

    show_leds();

}