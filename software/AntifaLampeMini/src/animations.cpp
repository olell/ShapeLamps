#include <stdint.h>
#include <Arduino.h>

#include "config.h"
#include "animations.h"
#include "driver.h"
#include "util.h"

const char* anim_off_name = "off";

uint8_t running_animation = OFF_ANIMATION;

float animation_speed = 127;

// rainbow animation
uint16_t rainbow_anim_hue = 0;

// clock animation
struct tm timeinfo;
uint32_t col;

void rainbow_animation() {
    set_segment_hsv(0, NUM_LEDS, (rainbow_anim_hue / 1000.0) * 255, 255, 255);
    rainbow_anim_hue += (animation_speed / 255.0) * 8;
    if (rainbow_anim_hue > 1000) rainbow_anim_hue = 0;
}

void rainbow_cycle_animation() {
    /*
    uint32_t col;
    float hue;
    for (int i = RING_FIRST; i <= RING_LAST; i ++) {
        hue = ((float)rainbow_anim_hue / 1000.0) + ((float) (i - RING_FIRST) / float(RING_LAST - RING_FIRST));
        if (hue >= 1) hue -= 1;
        col = hsv_to_rgb(hue * 360, 1, 1);
        colors[i] = col;

        if (i == 46) {
            for (int j = SMALL_FLAG_FIRST; j <= SMALL_FLAG_LAST; j++) {
                colors[j] = col;
            }
        }
        if (i == 75) {
            for (int j = BIG_FLAG_FIRST; j <= BIG_FLAG_LAST; j++) {
                colors[j] = col;
            }
        }
    }
    rainbow_anim_hue += animation_speed;
    if (rainbow_anim_hue > 1000) rainbow_anim_hue = 0;
    */
}

void single_color_animation() {
    /*if (single_color_type == 0) {
        set_segment_rgb(0, NUM_LEDS, single_color_all);
        }
    }
    if (single_color_type == 1) {
        for (int i = RING_FIRST; i <= RING_LAST; i ++)
            colors[i] = ring_color;
        for (int i = SMALL_FLAG_FIRST; i <= SMALL_FLAG_LAST; i ++)
            colors[i] = small_flag_color;
        for (int i = BIG_FLAG_FIRST; i <= BIG_FLAG_LAST; i ++)
            colors[i] = big_flag_color;
    }*/
}

void rainbow_bars_animation() {
    uint8_t hue;
    for (int y = 0; y < LED_GRID_HEIGHT; y ++) {
        hue = ((float) y / LED_GRID_HEIGHT) * 255;
        for (int x = 0; x < LED_GRID_WIDTH; x ++) {
            set_xy_hsv(x, y, hue, 255, 255);
        }
    }
    rainbow_anim_hue += (animation_speed / 255.0) * 8;
    if (rainbow_anim_hue > 1000) rainbow_anim_hue = 0;
}

void audio_visual_animation() {
}

void clock_animation() {
    /*getLocalTime(&timeinfo);

    Serial.println(timeinfo_boot_millis);*/
}

const char* anim_rainbow_name = "rainbow";
const char* anim_rainbow_cycle_name = "rainbow_cycle";
const char* anim_rainbow_bars_name = "rainbow_bars";
const char* anim_single_color_name = "single_color";
const char* anim_clock_name = "anim_clock";

void start_animation(const char* name) {
    log_info("Selected animation \"%s\"", name);
    set_segment_rgb(0, NUM_LEDS, 0, 0, 0);
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
}

void run_animations() {
    if (running_animation == OFF_ANIMATION) {
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

    if (running_animation == AUDIO_VISUAL) {
        audio_visual_animation();
    }

    if (running_animation == CLOCK) {
        clock_animation();
    }

    show_leds();
}