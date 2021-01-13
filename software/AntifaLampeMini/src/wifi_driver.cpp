#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>

#include "config.h"
#include "const.h"

#include "util.h"

IPAddress local_ip(10, 0, 0, 1);
IPAddress gateway_ip(10, 0, 0, 1);
IPAddress subnet_mask(255, 255, 255, 0);

const char* ap_ssid = WIFI_AP_SSID;
const char* ap_psk  = WIFI_AP_PSK;
char wifi_ssid[64] = WIFI_SSID;
char wifi_psk[64]  = WIFI_PSK;

void init_access_point() {
    log_debug("Starting AP");

    WiFi.mode(WIFI_AP);
    WiFi.enableAP(true);
    WiFi.softAP(ap_ssid, ap_psk);
    
    log_info("Started accesspoint, waiting a bit");
    delay(2000);
    log_info("Accesspoint should be ready soon");

    WiFi.softAPConfig(local_ip, gateway_ip, subnet_mask);
}

void load_credentials_from_fs() {
    File file = SPIFFS.open("/wifi_cred.txt");
    if (!file) {
        log_error("Couldn't open wifi credential file! (/wifi_cred.txt)");
    }
    uint8_t read_ptr = 0b00000000;
    char read;
    while(file.available()) {
        read = file.read();
        if (read == '\n') {
            if (read_ptr >> 7 == 1)
                break;
            read_ptr = 0b10000000;
        }
        else {
            if (read_ptr >> 7) { // psk
                wifi_psk[read_ptr & 0x7f] = read;
            }
            else { // ssid
                wifi_ssid[read_ptr & 0x7f] = read;
            }
            read_ptr ++;
        }
    }
    log_debug("Found SSID in SPIFFS: \"%s\"", wifi_ssid);
    log_debug("Found PSK in SPIFFS:  \"%s\"", wifi_psk);
    file.close();
}

void write_credentials_to_fs() {
    File file = SPIFFS.open("/wifi_cred.txt", "w");
    if(!file) {
        log_error("Couldn't open wifi credential file! (/wifi_cred.txt)");
    }
    char file_content[128];
    sprintf(file_content, "%s\n%s", wifi_ssid, wifi_psk);
    file.seek(0);
    file.print(file_content);
    file.close();
}

void wifi_init() {

    log_debug("Begin wifi driver initialisation");

    write_credentials_to_fs();
    load_credentials_from_fs();

    #if WIFI_BEHAVIOR == WIFI_AP_ONLY

    init_access_point();

    #elif WIFI_BEHAVIOR == WIFI_AP_CONFIG

    #elif WIFI_BEHAVIOR == WIFI_CONFIG_CRED

    #endif

}