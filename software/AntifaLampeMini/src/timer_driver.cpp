#include "Arduino.h"
#include <stdint.h>

#include "timer_driver.h"
#include "util.h"
#include "led_driver.h"
#include "animations.h"

int sleep_timer_set_time = 0;
int sleep_timer_dest_time = 0;
bool sleep_timer_enabled = false;
uint8_t sleep_timer_set_brightness = 0;


void timer_driver_init() {
    sleep_timer_enabled = false;
}

void start_sleep_timer(int minutes) {

    // ToDo: Maybe a rtc based approach is better?
    
    sleep_timer_enabled = true;
    sleep_timer_set_time = millis();
    sleep_timer_dest_time = sleep_timer_set_time + (60 * 1000 * minutes);
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
}