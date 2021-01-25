#pragma once

// Loglevels
#define LOGLEVEL_DEBUG 0
#define LOGLEVEL_INFO  1
#define LOGLEVEL_WARN  2
#define LOGLEVEL_ERROR 3
#define LOGLEVEL_FATAL 4
#define LOGLEVEL_OFF   5

// WiFi behaviors
// Just opens an access point
#define WIFI_AP_ONLY     0
// Opens an access point for configuration
#define WIFI_AP_CONFIG   1
// Connects to a configurated access point
#define WIFI_CONFIG_CRED 2

// Network stuff
#define DNS_PORT 53
#define HTTP_PORT 80

// Math stuff
#ifndef M_PI
#define M_PI 3.1415926
#endif

// Macros
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define random_float() (random(0, 1000) / 1000.0)