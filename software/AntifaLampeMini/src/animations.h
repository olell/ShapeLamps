#pragma once

#define OFF_ANIMATION           0
#define RAINBOW_ANIMATION       1
#define RAINBOW_CYCLE_ANIMATION 2
#define SINGLE_COLOR            3
#define RAINBOW_BARS            4
#define AUDIO_VISUAL            5
#define CLOCK                   6

void start_animation(const char* name);
void run_animations();