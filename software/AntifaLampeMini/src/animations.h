#pragma once

#define OFF_ANIMATION            0
#define RAINBOW_ANIMATION        1
#define RAINBOW_CYCLE_ANIMATION  2
#define SINGLE_COLOR             3
#define RAINBOW_BARS             4
#define CLOCK                    5
#define FIRE_ANIMATION           6
#define MATRIX_ANIMATION         7
#define RANDOM_FADE_ANIMATION    8
#define PLASMA_ANIMATION         9
#define EXTERNAL_ANIMATION      10

void start_animation(String name);
void run_animations();

void set_single_color(uint8_t r, uint8_t g, uint8_t b);

extern uint8_t running_animation;
extern float animation_speed;