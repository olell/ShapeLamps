#include <stdint.h>
#include <Arduino.h>
#include <stdarg.h>

#include "config.h"
#include "const.h"

const char* ANSI_BLACK   = "\u001b[30m";
const char* ANSI_RED     = "\u001b[31m";
const char* ANSI_GREEN   = "\u001b[32m";
const char* ANSI_YELLOW  = "\u001b[33m";
const char* ANSI_BLUE    = "\u001b[34m";
const char* ANSI_MAGENTA = "\u001b[35m";
const char* ANSI_CYAN    = "\u001b[36m";
const char* ANSI_WHITE   = "\u001b[37m";
const char* ANSI_RESET   = "\u001b[0m";


uint32_t last_tick_ms = millis();
uint32_t current_tick_ms = millis();

int8_t delay_time = 0;
float ms_per_cycle = 0;

char log_output_str[150];

uint8_t tick_fps(uint8_t fps) {

    uint8_t real_fps = fps;

    current_tick_ms = millis();

    ms_per_cycle = 1000.0 / fps;

    delay_time = ms_per_cycle - (current_tick_ms - last_tick_ms);

    if (delay_time > 0)
        delay(delay_time);
    else if(delay_time < 0) {
        real_fps = 1000 / (ms_per_cycle - delay_time);
    }

    last_tick_ms = millis();

    return real_fps;

}

// log functions
void log_debug(const char* val, ...) {
    #if LOGLEVEL <= LOGLEVEL_DEBUG
        va_list args;
        va_start(args, val);
        vsprintf(log_output_str, val, args);
        Serial.printf("[%sDEBUG%s]   %s%10d%s - %s\r\n", ANSI_GREEN, ANSI_RESET, ANSI_CYAN, millis(), ANSI_RESET, log_output_str);
    #endif
}
void log_info(const char* val, ...) {
    #if LOGLEVEL <= LOGLEVEL_INFO
        va_list args;
        va_start(args, val);
        vsprintf(log_output_str, val, args);
        Serial.printf("[%sINFO%s]    %s%10d%s - %s\r\n", ANSI_BLUE, ANSI_RESET, ANSI_CYAN, millis(), ANSI_RESET, log_output_str);
    #endif
}
void log_warn(const char* val, ...) {
    #if LOGLEVEL <= LOGLEVEL_WARN
        va_list args;
        va_start(args, val);
        vsprintf(log_output_str, val, args);
        Serial.printf("[%sWARNING%s] %s%10d%s - %s\r\n", ANSI_YELLOW, ANSI_RESET, ANSI_CYAN, millis(), ANSI_RESET, log_output_str);
    #endif
}
void log_error(const char* val, ...) {
    #if LOGLEVEL <= LOGLEVEL_ERROR
        va_list args;
        va_start(args, val);
        vsprintf(log_output_str, val, args);
        Serial.printf("[%sERROR%s]   %s%10d%s - %s\r\n", ANSI_RED, ANSI_RESET, ANSI_CYAN, millis(), ANSI_RESET, log_output_str);
    #endif
}
void log_fatal(const char* val, ...) {
    #if LOGLEVEL <= LOGLEVEL_FATAL
        va_list args;
        va_start(args, val);
        vsprintf(log_output_str, val, args);
        Serial.printf("[%sFATAL%s]   %s%10d%s - %s\r\n", ANSI_MAGENTA, ANSI_RESET, ANSI_CYAN, millis(), ANSI_RESET, log_output_str);
    #endif
}


void halt() {
    while (1) {
        delay(100);
    }
}