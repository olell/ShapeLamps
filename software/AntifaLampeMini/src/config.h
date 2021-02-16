#pragma once

#include <stdint.h>
#include "const.h"

// WiFi
#define WIFI_BEHAVIOR   WIFI_AP_CONFIG
#define WIFI_SSID       "" // Optional, only used in mode "WIFI_CONFIG_CRED"
#define WIFI_PSK        "" // Optional, only used in mode "WIFI_CONFIG_CRED"
#define WIFI_AP_SSID    "ShapeLamp0161" // Used in mode "WIFI_AP_ONLY" and "WIFI_AP_CONFIG"
#define WIFI_AP_PSK     "AFA1312!"      // Used in mode "WIFI_AP_ONLY" and "WIFI_AP_CONFIG"

#define WIFI_AP_IP      10,0,0,1 // Host IP Addr in AP mode (seperate segments by ',')
#define WIFI_HOSTNAME   "antifalampe"

#define WIFI_CONNECT_TIMEOUT 30000 // 30 seconds seems like a good value

// LED output
#define MAX_BRIGHTNESS  255
#define BOOT_BRIGHTNESS MAX_BRIGHTNESS

#define SYSTEM_FPS 60

// Logging
#define LOGLEVEL LOGLEVEL_DEBUG // DEBUG, INFO, WARN, ERROR, FATAL, OFF
#define FPS_DEBUG_PRINT_CYCLE 6000 // Print fps every ~xyz milliseconds

// Hardware, values suitable for Rev 1.0
#define LED_PIN 4
#define NUM_LEDS 90

#define SPEAKER_PIN 25
#define ENABLE_SOUND // Comment this line to disable sound output

#define BIG_FLAG_FIRST_LED  0
#define BIG_FLAG_LAST_LED  30
#define SMALL_FLAG_FIRST_LED   31
#define SMALL_FLAG_LAST_LED    45
#define RING_FIRST_LED       46
#define RING_LAST_LED        89

#define LED_GRID_WIDTH   20
#define LED_GRID_HEIGHT  20
#define LED_GRID_PER_CELL 3

extern uint16_t led_map[NUM_LEDS];
extern int8_t led_grid[20][20][3];

// OTA update
extern const char* firmware_url;
extern const char* spiffs_url;
extern const char* server_cert;

// NTP
#define NTP_SERVER "pool.ntp.org"
#define NTP_NORMAL_OFFSET 3600
#define NTP_DST_OFFSET 3600

// config specific functions
void log_config();