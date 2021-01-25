#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>

#include <ESPAsyncWebServer.h>

#include "http_driver.h"
#include "util.h"
#include "const.h"
#include "config.h"
#include "wifi_driver.h"
#include "driver.h"
#include "animations.h"
#include "ota_driver.h"

AsyncWebServer server(HTTP_PORT);

String dummy_response = "done!";

// Views only for WIFI_BEHAVIOR == WIFI_AP_CONFIG

#if WIFI_BEHAVIOR == WIFI_AP_CONFIG
void view_scan_networks(AsyncWebServerRequest *request) {
    String response_json = "{";

    int num = WiFi.scanNetworks();

    if (num == 0) {
        response_json += "\"status\":\"error\",";
        response_json += "\"message\":\"No networks found\"";
    }

    else {
        response_json += "\"status\":\"ok\",";
        response_json += "\"networks\":[";
        for (int i = 0; i < num; i++) {
            response_json += '"' + WiFi.SSID(i) + "\",";
        }
        response_json[response_json.length() - 1] = ']';
    }

    response_json += '}';
    request->send(200, "application/json", response_json);
}

void view_wifi_configurator(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/html/cred_config.html", "text/html");
}

void view_set_wifi_credentials(AsyncWebServerRequest *request) {
    String wifi_ssid = "";
    String wifi_pass = "";

    bool all_params = true;

    if (request->hasParam("ssid", true)) {
        AsyncWebParameter* ssid_param = request->getParam("ssid", true);
        wifi_ssid = ssid_param->value();
    } else all_params = false;

    if (request->hasParam("psk", true)) {
        AsyncWebParameter* psk_param = request->getParam("psk", true);
        wifi_pass = psk_param->value();
    } else all_params = false;


    if (!all_params) {
        request->send(200, "text/plain", "Missing parameter/s, please check your request!");
    }

    set_wifi_credentials(wifi_ssid.c_str(), wifi_pass.c_str());
    write_credentials_to_fs();

    log_debug("Configured wifi ssid: %s", wifi_ssid.c_str());
    log_debug("Configured wifi pass: %s", wifi_pass.c_str());

    request->redirect("/reboot");

}
#endif

void view_reboot(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/html/reboot.html", "text/html");
}

void view_reboot_do(AsyncWebServerRequest *request) {
    request->redirect("http://antifalampe/");
    delay(1000);
    ESP.restart();
}

void view_root(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/html/index.html", "text/html");
}

void view_get_basic(AsyncWebServerRequest *request) {
    String response_json = "{";
    response_json += "\"brightness\": " + String(brightness) + ",";
    response_json += "\"normalize\": " + (normalize ? String("true") : String("false")) + ",";
    response_json += "\"animation_speed\": " + String(animation_speed)  + "}";
    request->send(200, "application/json", response_json);
}

void view_set_basic(AsyncWebServerRequest *request) {
    if (request->hasParam("brightness", true)) {
        AsyncWebParameter* brightness_param = request->getParam("brightness", true);
        set_brightness(brightness_param->value().toInt());
    }

    if (request->hasParam("normalize", true)) {
        AsyncWebParameter* normalize_param = request->getParam("normalize", true);
        set_normalize(normalize_param->value() == "true");
    }

    if (request->hasParam("animation_speed", true)) {
        AsyncWebParameter* animation_speed_param = request->getParam("animation_speed", true);
        animation_speed = animation_speed_param->value().toInt();
    }
    
    request->send(200, "text/plain", dummy_response); // Answer will be ignored in javascript
}

void view_start_animation(AsyncWebServerRequest *request) {
    if (request->hasParam("name", true)) {
        AsyncWebParameter* name_param = request->getParam("name", true);
        start_animation(name_param->value());
    }
    request->send(200, "text/plain", dummy_response);
}

void view_set_single_color(AsyncWebServerRequest *request) {
    if (request->hasParam("color", true)) {
        AsyncWebParameter* color_param = request->getParam("color", true);
        const char* color = color_param->value().c_str();
        uint8_t rr, rg, rb;
        sscanf(color, "#%02x%02x%02x", &rr, &rg, &rb);
        set_single_color(rr, rg, rb);
    }
    request->send(200, "text/plain", dummy_response);
}

void view_get_power_consumption(AsyncWebServerRequest *request) {
    String response_json = "{\"power_ma\": " + String(get_current_power_consumption()) + "}";
    request->send(200, "application/json", response_json);
}

void serve_static(AsyncWebServerRequest *request) {
    // todo: this might have some security impact!!!
    String dir_name = request->pathArg(0);
    String file_name = request->pathArg(1);

    if (dir_name.equals("..") || dir_name.equals("/") || file_name.equals("..") || file_name.equals("/")) {
        request->send(200, "text/plain", "Nahh, don't do this you silly, bad boy!");
    }

    String path = "/static/" + dir_name + "/" + file_name;
    String mime;
    
    if (dir_name == "css") mime = "text/css";
    if (dir_name == "js") mime = "application/javascript";
    if (dir_name == "svg") mime = "image/svg+xml";
    if (dir_name == "png") mime = "image/png";

    log_debug("Handling request for static file %s", path.c_str());
    request->send(SPIFFS, path, mime);
}

void view_do_ota_update(AsyncWebServerRequest *request) {
    schedule_ota();
    request->redirect("/reboot");
}

void http_init() {
    log_debug("Starting to init the http driver");

    // Static file serving
    server.on("^\\/static\\/(.*)\\/(.*)$", HTTP_GET, serve_static);
    
    #if WIFI_BEHAVIOR == WIFI_AP_CONFIG || WIFI_BEHAVIOR == WIFI_AP_ONLY
        server.on("/scan_networks", HTTP_GET, view_scan_networks);
        server.on("/wifi_config", HTTP_GET, view_wifi_configurator);
        server.on("/set_wifi_credentials", HTTP_POST, view_set_wifi_credentials);
    #endif

    server.on("/", HTTP_GET, view_root);
    // server.on("/index.html", HTTP_GET, view_root);

    server.on("/get_basic", HTTP_GET, view_get_basic);
    server.on("/set_basic", HTTP_POST, view_set_basic);
    server.on("/start_animation", HTTP_POST, view_start_animation);
    server.on("/set_single_color", HTTP_POST, view_set_single_color);
    server.on("/get_power_consumption", HTTP_GET, view_get_power_consumption);

    server.on("/reboot", HTTP_GET, view_reboot);
    server.on("/do_reboot", HTTP_GET, view_reboot_do);

    server.on("/do_ota_update", HTTP_GET, view_do_ota_update);

    server.begin();
    log_debug("Started http server");

}
