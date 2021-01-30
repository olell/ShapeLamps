#include "Arduino.h"
#include <stdint.h>
#include "AsyncUDP.h"

#include "led_driver.h"
#include "config.h"
#include "const.h"
#include "util.h"
#include "animations.h"

// ToDo: external documentation
// ToDo: Maybe do some checksuming?
// Each packet needs to be (NUM_LEDS * 3) + 1 bytes large!
// Commands:
// 0: Clear
// 1: Set Brightness (2nd byte)
// 2: Set Pixels rgb (90 * rgb) + show
// 3: Set Pixels hsv (90 * hsv) + show
// 4: Shift pixels (2nd byte: 1st bit: left/right(0/1) next 7-bits: cycles)
uint8_t* udp_anim_recv; // 1st byte: cmd, then three values per pixel
AsyncUDP udp;

void udp_callback(AsyncUDPPacket packet) {

    if (running_animation == EXTERNAL_ANIMATION) {

        uint8_t cmd = 0;

        if (packet.length() == (NUM_LEDS * 3) + 1) {

            memcpy(udp_anim_recv, packet.data(), packet.length());

            cmd = udp_anim_recv[0];

            if (cmd == 0) {
                set_segment_rgb(0, NUM_LEDS, 0, 0, 0);
                return;
            }
            else if (cmd == 1) {
                set_brightness(udp_anim_recv[1]);
                return;
            }
            else if (cmd == 2) {
                for (int i = 0; i < NUM_LEDS; i ++) {
                    set_led_rgb(i, 
                        udp_anim_recv[1 + (3 * i)],
                        udp_anim_recv[2 + (3 * i)],
                        udp_anim_recv[3 + (3 * i)]);
                }
                show_leds();
                return;
            }
            else if (cmd == 3) {
                for (int i = 0; i < NUM_LEDS; i ++) {
                    set_led_hsv(i, 
                        udp_anim_recv[1 + (3 * i)],
                        udp_anim_recv[2 + (3 * i)],
                        udp_anim_recv[3 + (3 * i)]);
                }
                show_leds();
                return;
            }
            else if (cmd == 4){
                // todo
                return;
            }
        }

    }
}

void udp_init() {
    if (udp.listen(UDP_PORT)) {
        udp_anim_recv = (uint8_t*) malloc((NUM_LEDS * 3) + 1);
        log_debug("UDP listening!");
        udp.onPacket(udp_callback);
    }
}