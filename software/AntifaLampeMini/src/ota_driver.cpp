#include "WiFi.h"
#include "HttpsOTAUpdate.h"
#include "SPIFFS.h"
#include "HTTPClient.h"
#include "WiFiClientSecure.h"

#include "util.h"
#include "ota_driver.h"
#include "animations.h"
#include "driver.h"
#include "config.h"
#include "wifi_driver.h"

static HttpsOTAStatus_t otastatus;

bool is_ota_scheduled = false;
bool is_ota_running = false;

void http_event(HttpEvent_t *event){
    switch(event->event_id) {
        case HTTP_EVENT_ERROR:
            log_error("OTA: Http Event Error");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            log_debug("Http Event On Connected");
            break;
        case HTTP_EVENT_HEADER_SENT:
            log_debug("Http Event Header Sent");
            break;
        case HTTP_EVENT_ON_HEADER:
            log_debug("Http Event On Header, key=%s, value=%s", event->header_key, event->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            break;
        case HTTP_EVENT_ON_FINISH:
            log_debug("Http Event On Finish");
            break;
        case HTTP_EVENT_DISCONNECTED:
            log_debug("Http Event Disconnected");
            break;
    }
}

void ota_init() {
    log_debug("Initing ota driver");
    HttpsOTA.onHttpEvent(http_event);
}

void schedule_ota() {
    if (!is_ota_scheduled && !is_ota_running) is_ota_scheduled = true;
}

void ota_schedule_loop() {
    if (is_ota_scheduled) {
        is_ota_scheduled = false;
        ota_update();
    }
}

bool ota_update_firmware() {
    HttpsOTA.begin(firmware_url, server_cert, false);
    
    log_info("Starting OTA firmware update");
    
    bool success = false;

    while (1) {

        otastatus = HttpsOTA.status();

        if (otastatus == HTTPS_OTA_SUCCESS) {
            log_info("OTA Update successful!");
            success = true;
            break;
        }

        if (otastatus == HTTPS_OTA_FAIL) {
            log_error("OTA Update failed!");
            success = false;
            break;
        }

        do_cycle(0.1);

    }
    return success;
}
bool ota_update_spiffs() {
    bool success = false;

    load_credentials_from_fs(); // to backup wifi credentials temp. in RAM

    SPIFFS.end(); // Unmount
    
    const esp_partition_t* partition = esp_partition_get(esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, NULL));
    log_debug("SPIFFS_OTA: found spiffs partition @ %d, size: %d", partition->address, partition->size);

    HTTPClient http;
    http.begin(spiffs_url, server_cert);
    log_debug("SPIFFS_OTA http begin");
    int response_code = http.GET();
    log_debug("SPIFFS_OTA: http response %d", response_code);
    if (response_code == 200) {
        log_debug("SPIFFS_OTA: http, 200");

        uint8_t buf[1024] = { 0 };

        WiFiClient* stream = http.getStreamPtr();

        int total = http.getSize();
        int len = total;

        int offset = 0;

        // Erase spiffs partition
        esp_partition_erase_range(partition, 0, partition->size);

        while (http.connected() && (stream->available() > -1) && (len > 0 || len == -1)) {
            size_t size = stream->available();
            if (size) {
                int c = stream->readBytes(buf,((size > sizeof(buf)) ? sizeof(buf) : size ));

                esp_partition_write(partition, offset, buf, c);
                offset += c;
                if (len > 0) len -= c;
            }

            display_ring_percent((total - len) / (float) total, 255, 255, 255);
        }
        success = true;
    }
    else {
        success = false;
    }

    if (success) { // restore wifi credentials
        if(!SPIFFS.begin(true)) { // remount
            log_fatal("SPIFFS_OTA: An fatal error occured while mounting SPIFFS. Please reset controller!");
            success = false;
        }
        else {
            log_debug("SPIFFS_OTA: Mounted SPIFFS");
            write_credentials_to_fs();
        }
    }

    http.end();
    return success;
}

bool ota_update() {
    is_ota_running = true;
    
    bool success = ota_update_spiffs() & ota_update_firmware();

    if (success) {
        set_single_color(0, 255, 0);
    }
    else {
        set_single_color(255, 0, 0);
    }

    animation_speed = 255.0;
    set_brightness(255);
    start_animation("single_color");
    run_animations();
    show_leds();

    is_ota_running = false;

    if (success) {
        delay(1000);
        ESP.restart();
    }

    return success;

}