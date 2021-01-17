#pragma once

#include <stdint.h>
#include <Arduino.h>

uint8_t tick_fps(uint8_t fps);

void log_debug(const char* val, ...);
void log_info(const char* val, ...);
void log_warn(const char* val, ...);
void log_error(const char* val, ...);
void log_fatal(const char* val, ...);

void halt();

String ip_to_string(IPAddress ip);