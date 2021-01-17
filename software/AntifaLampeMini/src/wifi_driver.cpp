#include <Arduino.h>
#include <WiFi.h>
#if WIFI_BEHAVIOR == WIFI_AP_ONLY || WIFI_BEHAVIOR == WIFI_AP_CONFIG
    #include "DNSServer.h"
#endif
#include <SPIFFS.h>

#include "config.h"
#include "const.h"

#include "util.h"

IPAddress local_ip(WIFI_AP_IP);
IPAddress gateway_ip(WIFI_AP_IP);
IPAddress subnet_mask(255, 255, 255, 0);

// If not defined in configuration: (to prevent errors)
#ifndef WIFI_AP_SSID
    #define WIFI_AP_SSID ""
#endif
#ifndef WIFI_AP_PSK
    #define WIFI_AP_PSK ""
#endif

const char* ap_ssid = WIFI_AP_SSID;
const char* ap_psk  = WIFI_AP_PSK;
char wifi_ssid[64] = WIFI_SSID;
char wifi_psk[64]  = WIFI_PSK;

#if WIFI_BEHAVIOR == WIFI_AP_ONLY || WIFI_BEHAVIOR == WIFI_AP_CONFIG
    DNSServer dns_server;
#endif

bool connect_to_network() {
    // Call this method only if WIFI_BEHAVIOR is "WIFI_CONFIG_CRED" or "WIFI_AP_CONFIG"
    log_info("Starting to connect to network \"%s\"", wifi_ssid);

    WiFi.mode(WIFI_STA);
    WiFi.enableAP(false);
    WiFi.begin(wifi_ssid, wifi_psk);

    bool connected = false;

    uint8_t prev_status = 0;
    uint8_t current_status = 0;

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);

        prev_status = current_status;
        current_status = WiFi.status();
        if (current_status != prev_status) {
            if (current_status == WL_DISCONNECTED) {
                // Do nothing, just waiting for connection
            }
            else if (current_status == WL_CONNECTED) {
                log_info("Connection successfully established!");
                log_info("My IP address is: %s", WiFi.localIP().toString().c_str());
                connected = true;
                break;
            }
            else if (current_status == WL_NO_SSID_AVAIL) {
                log_warn("SSID \"%s\" not found...", wifi_ssid);
                connected = false;
                break;
            }
            else if (current_status == WL_CONNECT_FAILED) {
                log_warn("WiFi connection failed, maybe invalid credentials?");
                connected = false;
                break;
            }
        }
        
    }

    return connected;

}

#if WIFI_BEHAVIOR == WIFI_AP_ONLY || WIFI_BEHAVIOR == WIFI_AP_CONFIG
void init_access_point() {
    log_info("Starting AP");

    WiFi.mode(WIFI_AP);
    WiFi.enableAP(true);
    WiFi.softAP(ap_ssid, ap_psk);
    
    log_info("Started accesspoint, waiting a bit");
    delay(2000);
    log_info("Accesspoint should be ready soon");

    WiFi.softAPConfig(local_ip, gateway_ip, subnet_mask);
    log_debug("AP started and configurated!");

    dns_server.start(DNS_PORT, "*", local_ip);
    log_info("DNS Server started!");
    
}
#endif

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


    #if WIFI_BEHAVIOR == WIFI_AP_ONLY

    init_access_point();

    #elif WIFI_BEHAVIOR == WIFI_AP_CONFIG

    load_credentials_from_fs();

    #elif WIFI_BEHAVIOR == WIFI_CONFIG_CRED

    connect_to_network();

    #endif

}