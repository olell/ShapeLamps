#include <stdint.h>

#include "config.h"
#include "util.h"

uint16_t led_map[NUM_LEDS] = {0x333b,0x294a,0x215a,0x1b6c,0x237c,0x3385,0x448c,0x568e,0x678c,0x7988,0x8b85,0x9c89,0xac91,0xbe96,0xb9a8,0xb3b9,0xadca,0xa7db,0xc586,0xcb75,0xd164,0xd753,0xcb46,0xbc3c,0xaa38,0x993e,0x8845,0x7749,0x654a,0x5349,0x4244,0x8894,0x7698,0x659c,0x539c,0x419b,0x2f9a,0x218f,0x3ba8,0x4cae,0x5eae,0x70ae,0x82b3,0x8ca4,0x81c5,0x7bd6,0x6ffe,0x5dfa,0x4cf4,0x3beb,0x2de0,0x20d4,0x14c5,0x0bb5,0x05a4,0x0192,0x0080,0x016d,0x055b,0x0b4a,0x143a,0x1f2b,0x2c1e,0x3a13,0x4b0b,0x5c04,0x6e01,0x8100,0x9301,0xa505,0xb60c,0xc615,0xd420,0xe12e,0xec3d,0xf44d,0xfa5e,0xfe70,0xff82,0xfd94,0xf9a5,0xf2b6,0xe9c6,0xded4,0xd1e1,0xc2eb,0xb2f4,0xa3f9,0x92fd,0x80ff};
int8_t led_grid[20][20][3] = {{{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {46, -1, -1}, {89, -1, -1}, {88, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {49, -1, -1}, {48, -1, -1}, {47, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {87, -1, -1}, {86, -1, -1}, {85, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {50, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {45, -1, -1}, {-1, -1, -1}, {17, -1, -1}, {-1, -1, -1}, {84, -1, -1}, {83, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {52, -1, -1}, {51, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {44, -1, -1}, {-1, -1, -1}, {16, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {82, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {53, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {39, -1, -1}, {40, -1, -1}, {41, -1, -1}, {42, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {15, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {81, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {54, -1, -1}, {-1, -1, -1}, {38, -1, -1}, {35, -1, -1}, {34, -1, -1}, {33, -1, -1}, {-1, -1, -1}, {43, -1, -1}, {-1, -1, -1}, {14, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {80, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{55, -1, -1}, {-1, -1, -1}, {37, -1, -1}, {36, -1, -1}, {6, -1, -1}, {7, -1, -1}, {8, -1, -1}, {9, 31, 32}, {-1, -1, -1}, {11, 12, -1}, {13, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {79, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{56, -1, -1}, {-1, -1, -1}, {4, -1, -1}, {5, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {10, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {18, 19, -1}, {-1, -1, -1}, {-1, -1, -1}, {78, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{57, -1, -1}, {-1, -1, -1}, {3, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {20, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {77, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {58, -1, -1}, {2, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {21, -1, -1}, {76, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {59, -1, -1}, {-1, -1, -1}, {0, 1, -1}, {30, -1, -1}, {29, -1, -1}, {28, -1, -1}, {26, 27, -1}, {25, -1, -1}, {-1, -1, -1}, {23, -1, -1}, {22, -1, -1}, {-1, -1, -1}, {74, 75, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {60, -1, -1}, {61, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {24, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {73, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {62, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {71, 72, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {63, -1, -1}, {64, -1, -1}, {65, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {68, -1, -1}, {69, -1, -1}, {70, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {66, -1, -1}, {67, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}, {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}};

const char* firmware_url = "https://olel.de/antifalampe/firmware.bin";
const char* spiffs_url = "https://olel.de/antifalampe/spiffs.bin";

const char* server_cert = "-----BEGIN CERTIFICATE-----\n" \
"MIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw\n" \
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw\n" \
"WhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\n" \
"RW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
"AoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP\n" \
"R5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx\n" \
"sxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm\n" \
"NHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg\n" \
"Z3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG\n" \
"/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB\n" \
"Af8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA\n" \
"FHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw\n" \
"AoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw\n" \
"Oi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB\n" \
"gt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W\n" \
"PTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl\n" \
"ikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz\n" \
"CkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm\n" \
"lJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4\n" \
"avAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2\n" \
"yJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O\n" \
"yK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids\n" \
"hCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+\n" \
"HlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv\n" \
"MldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX\n" \
"nLRbwHOoq7hHwg==\n" \
"-----END CERTIFICATE-----";

void log_config() {
    // dumps configuration to debug log
    log_debug("CONFIGURATION DUMP:");
    log_debug("");
    log_debug("WIFI:");
    #if WIFI_BEHAVIOR == WIFI_AP_ONLY
    log_debug("  WIFI_BEHAVIOR           WIFI_AP_ONLY");
    #elif WIFI_BEHAVIOR == WIFI_AP_CONFIG
    log_debug("  WIFI_BEHAVIOR           WIFI_AP_CONFIG");
    #elif WIFI_BEHAVIOR == WIFI_CONFIG_CRED
    log_debug("  WIFI_BEHAVIOR           WIFI_CONFIG_CRED");
    #endif
    log_debug("  WIFI_SSID               %s", WIFI_SSID);
    log_debug("  WIFI_PSK                %s", WIFI_PSK);
    log_debug("  WIFI_AP_SSID            %s", WIFI_AP_SSID);
    log_debug("  WIFI_AP_PSK             %s", WIFI_AP_PSK);
    log_debug("  WIFI_AP_IP              %d.%d.%d.%d", WIFI_AP_IP);
    log_debug("  WIFI_HOSTNAME           %s", WIFI_HOSTNAME);
    log_debug("  WIFI_CONNECT_TIMEOUT    %d", WIFI_CONNECT_TIMEOUT);
    log_debug("LED OUTPUT:");
    log_debug("  MAX_BRIGHTNESS:         %d", MAX_BRIGHTNESS);
    log_debug("  BOOT_BRIGHTNESS:        %d", BOOT_BRIGHTNESS);
    log_debug("  SYSTEM_FPS:             %d", SYSTEM_FPS);
    log_debug("");
    log_debug("LOGGING:");
    log_debug("  LOGLEVEL:               DEBUG (obviously :P)");
    log_debug("  FPS_DEBUG_PRINT_CYCLE:  %d", FPS_DEBUG_PRINT_CYCLE);
    log_debug("");
    log_debug("HARDWARE:");
    log_debug("  LED_PIN                 %d", LED_PIN);
    log_debug("  NUM_LEDS                %d", NUM_LEDS);
    log_debug("");
    log_debug("  MAPPINGS:");
    log_debug("    SMALL_FLAG_FIRST_LED  %d", SMALL_FLAG_FIRST_LED);
    log_debug("    SMALL_FLAG_LAST_LED   %d", SMALL_FLAG_LAST_LED);
    log_debug("    BIG_FLAG_FIRST_LED    %d", BIG_FLAG_FIRST_LED);
    log_debug("    BIG_FLAG_LAST_ LED    %d", BIG_FLAG_LAST_LED);
    log_debug("    RING_FIRST_LED        %d", RING_FIRST_LED);
    log_debug("    RING_FLAG_LAST_LED    %d", RING_LAST_LED);
    log_debug("    LED_GRID_WIDTH        %d", LED_GRID_WIDTH);
    log_debug("    LED_GRID_HEIGHT       %d", LED_GRID_HEIGHT);
    log_debug("    LED_GRID_PER_CELL     %d", LED_GRID_PER_CELL);

    log_debug("");
    log_debug("ESTIMATED LED POSITIONS:");
    log_debug("");

    char mapping_draw[66];
    mapping_draw[65] = 0x00; // Termination

    uint8_t x, y;
    for (int line = 0; line < 32; line ++){
        for (int x = 0; x < 65; x++){
            mapping_draw[x] = ' ';
        }
        for (int i = 0; i < NUM_LEDS; i++) {
            x = led_map[i] >> 8;
            y = led_map[i] & 0xff;
            if ((y/8) == line) {
                mapping_draw[x/4] = '#';
            }
        }
        log_debug(mapping_draw);
    }

    log_debug("");

}