#pragma once

#include <stdint.h>

void timer_driver_init();

void start_sleep_timer(int minutes);
void stop_sleep_timer();
uint32_t get_sleeptimer_left_millis();
bool get_sleeptimer_running();

void start_wake_timer(struct tm start_time, int minutes);
void stop_wake_timer();

void update_timer();