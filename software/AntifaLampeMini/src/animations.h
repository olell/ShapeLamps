#pragma once

#define OFF_ANIMATION           0
#define RAINBOW_ANIMATION       1
#define RAINBOW_CYCLE_ANIMATION 2
#define SINGLE_COLOR            3
#define RAINBOW_BARS            4
// #define AUDIO_VISUAL            5
#define CLOCK                   6
#define FIRE_ANIMATION          7
#define MATRIX_ANIMATION        8

void start_animation(String name);
void run_animations();

void set_single_color(uint8_t r, uint8_t g, uint8_t b);

extern uint8_t running_animation;
extern float animation_speed;