#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "const.h"

#include "util.h"

IPAddress local_ip(10, 0, 0, 1);
IPAddress gateway_ip(10, 0, 0, 1);
IPAddress subnet_mask(255, 255, 255, 0);

const char* ap_ssid = WIFI_AP_SSID;
const char* ap_psk  = WIFI_AP_PSK;
const char* wifi_ssid = WIFI_SSID;
const char* wifi_psk  = WIFI_PSK;

void init_access_point() {
    log_debug("Starting AP");

    WiFi.mode(WIFI_AP);
    WiFi.enableAP(true);
    WiFi.softAP(ap_ssid, ap_psk);
    WiFi.softAPConfig(local_ip, gateway_ip, subnet_mask);
    
    log_info("Started accesspoint, waiting a bit");
    delay(500);
    log_info("Accesspoint should be ready!");
}

void wifi_init() {

    log_debug("Begin wifi driver initialisation");

    #if WIFI_BEHAVIOR == WIFI_AP_ONLY

    init_access_point();

    #elif WIFI_BEHAVIOR == WIFI_AP_CONFIG

    #elif WIFI_BEHAVIOR == WIFI_CONFIG_CRED

    #endif

}