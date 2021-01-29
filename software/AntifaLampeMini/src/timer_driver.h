#pragma once

#include <stdint.h>

void timer_driver_init();

void start_sleep_timer(int minutes);
void stop_sleep_timer();
uint32_t get_sleeptimer_left_millis();
bool get_sleeptimer_running();

void update_timer();