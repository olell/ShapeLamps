#pragma once

#include <stdint.h>
#include <Arduino.h>

uint8_t tick_fps(uint8_t fps);

void enable_frame_limit();
void disable_frame_limit();

void log_debug(const char* val, ...);
void log_info(const char* val, ...);
void log_warn(const char* val, ...);
void log_error(const char* val, ...);
void log_fatal(const char* val, ...);

void halt();

void init_time();
struct tm get_time();

String ip_to_string(IPAddress ip);