#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>

#include <ESPAsyncWebServer.h>

#include "http_driver.h"
#include "util.h"
#include "const.h"
#include "config.h"

AsyncWebServer server(HTTP_PORT);

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

    log_debug("Handling request for static file %s", path.c_str());
    request->send(SPIFFS, path, mime);
}

void http_init() {
    
    log_debug("Starting to init the http driver");

    // Static file serving
    server.on("^\\/static\\/(.*)\\/(.*)$", HTTP_GET, serve_static);
    
    //#if WIFI_BEHAVIOR == WIFI_AP_CONFIG || WIFI_BEHAVIOR == WIFI_AP_ONLY
        server.on("/scan_networks", HTTP_GET, view_scan_networks);
    //#endif

    server.begin();
    log_debug("Started http server");

}
