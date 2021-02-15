#include "Arduino.h"
#include <stdint.h>

#include "timer_driver.h"
#include "util.h"
#include "led_driver.h"
#include "animations.h"

unsigned long sleep_timer_set_time = 0;
unsigned long sleep_timer_dest_time = 0;
bool sleep_timer_enabled = false;
uint8_t sleep_timer_set_brightness = 0;

unsigned long wake_timer_set_time = 0;
unsigned long wake_timer_dest_time = 0;
bool wake_timer_enabled = false;

int wake_timer_minutes = 0;
struct tm wake_timer_start_time;

void timer_driver_init() {
    sleep_timer_enabled = false;
}

void start_wake_timer(struct tm start_time, int minutes) {
    wake_timer_start_time = start_time;
    wake_timer_minutes = minutes;

    wake_timer_enabled = true;
}

void stop_wake_timer() {
    wake_timer_set_time = 0;
    wake_timer_enabled = false;
}

void update_wake_timer() {
    if (wake_timer_set_time == 0) {
        struct tm now = get_time();
        if (now.tm_hour == wake_timer_start_time.tm_hour && now.tm_min == wake_timer_start_time.tm_min) {
            wake_timer_set_time = millis();
            wake_timer_dest_time = wake_timer_set_time + (60000 * wake_timer_minutes);
        }
    }
    else {
        unsigned long now = millis();
        if (now <= wake_timer_dest_time) {
            uint8_t new_brightness = (1 - ((wake_timer_dest_time - now) / (float)(wake_timer_dest_time - wake_timer_set_time))) * 255;
            set_brightness(new_brightness);
        }
        else {
            stop_wake_timer();
        }
    }
}

bool get_wake_timer_enabled() {
    return wake_timer_enabled;
}

struct tm get_wake_timer_start_time() {
    return wake_timer_start_time;
}

int get_wake_timer_minutes() {
    return wake_timer_minutes;
}

void start_sleep_timer(int minutes) {

    // ToDo: Maybe a rtc based approach is better?
    
    sleep_timer_enabled = true;
    sleep_timer_set_time = millis();
    sleep_timer_dest_time = sleep_timer_set_time + (60000 * minutes);
    sleep_timer_set_brightness = brightness;

}

void stop_sleep_timer() {
    if(sleep_timer_enabled) {
        sleep_timer_enabled = false;
        set_brightness(sleep_timer_set_brightness);
    }
}

void update_sleep_timer() {
    unsigned long now = millis();
    if (now >= sleep_timer_dest_time) {
        set_brightness(0);
        sleep_timer_enabled = false;
    }
    else if (now > (sleep_timer_dest_time - 60000)) { // Last minute fadeout
        float p = (now - (sleep_timer_dest_time - 60000)) / 60000.;
        set_brightness(sleep_timer_set_brightness * (1-p));
    }
}

uint32_t get_sleeptimer_left_millis() {
    return sleep_timer_dest_time - millis();
}

bool get_sleeptimer_running() {
    return sleep_timer_enabled;
}

void update_timer() {
    if(sleep_timer_enabled)
        update_sleep_timer();
    if(wake_timer_enabled)
        update_wake_timer();
}