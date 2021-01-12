#include "driver.h"
#include "config.h"
#include "util.h"
#include "animations.h"

int fps_print_last = millis();
uint8_t real_fps = 0;

void setup() {
    Serial.begin(115200);
    log_info("System booted");

    log_config();

    driver_init();
    log_debug("LED driver inited");
    log_info("Initialisation done!");

    start_animation("rainbow_bars");
    
}

void loop() {

    run_animations();

    // delay(100);

    real_fps = tick_fps(SYSTEM_FPS);
    if(millis() - fps_print_last > FPS_DEBUG_PRINT_CYCLE) {
        log_debug("Running at %d fps. (SYSTEM_FPS: %d)", real_fps, SYSTEM_FPS);
        fps_print_last = millis();
    }

}