#pragma once

#include <stdint.h>

void sound_driver_init();
void beep(int ms);

void enable_alarm();
void update_alarm();
void disable_alarm();