#include <SPIFFS.h>

#include "led_driver.h"
#include "config.h"
#include "util.h"
#include "animations.h"
#include "wifi_driver.h"
#include "http_driver.h"
#include "ota_driver.h"

int fps_print_last = millis();
uint8_t real_fps = 0;

void setup() {
    Serial.begin(115200);
    log_info("System booted");

    log_config();

    if(!SPIFFS.begin(true)) {
        log_fatal("An fatal error occured while mounting SPIFFS. Please reset controller!");
        halt();
    }
    log_debug("Mounted SPIFFS");
    
    driver_init();
    log_debug("LED driver inited");
    wifi_init();
    log_debug("WiFi driver inited");
    init_time();
    log_debug("Time inited");
    http_init();
    log_debug("Http driver inited");
    ota_init();
    log_debug("OTA update driver inited");

    log_info("Initialisation done!");

    start_animation("dev");

    set_normalize(false);

    get_time();

}

void loop() {

    run_animations();

    ota_schedule_loop();

    real_fps = tick_fps(SYSTEM_FPS);
    if(millis() - fps_print_last > FPS_DEBUG_PRINT_CYCLE) {
        log_debug("Running at %d fps. (SYSTEM_FPS: %d)", real_fps, SYSTEM_FPS);
        fps_print_last = millis();
    }
    
}