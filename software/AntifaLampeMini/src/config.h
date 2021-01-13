#pragma once

#include <stdint.h>
#include "const.h"

// WiFi
#define WIFI_BEHAVIOR   WIFI_CONFIG_CRED
#define WIFI_SSID       "Your SSID"
#define WIFI_PSK        "Your PSK"
#define WIFI_AP_SSID    "Your AP Name"
#define WIFI_AP_PSK     "Your AP Pass"

// LED output
#define MAX_BRIGHTNESS  255
#define BOOT_BRIGHTNESS MAX_BRIGHTNESS

#define SYSTEM_FPS 60

// Logging
#define LOGLEVEL LOGLEVEL_INFO // DEBUG, INFO, WARN, ERROR, FATAL, OFF
#define FPS_DEBUG_PRINT_CYCLE 6000 // Print fps every ~xyz milliseconds

// Hardware, values suitable for Rev 1.0
#define LED_PIN 16
#define NUM_LEDS 90

#define SMALL_FLAG_FIRST_LED  0
#define SMALL_FLAG_LAST_LED  30
#define BIG_FLAG_FIRST_LED   31
#define BIG_FLAG_LAST_LED    45
#define RING_FIRST_LED       46
#define RING_LAST_LED        89

#define LED_GRID_WIDTH   20
#define LED_GRID_HEIGHT  20
#define LED_GRID_PER_CELL 3

extern uint16_t led_map[NUM_LEDS];
extern int8_t led_grid[20][20][3];

// config specific functions
void log_config();