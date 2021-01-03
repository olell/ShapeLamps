#include <Adafruit_ILI9341.h>
#include <WiFi.h>
#include "arduinoFFT.h"

#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

#include "led_map.h"

#define TFT_CS 5
#define TFT_DC 22

#define AUDIO_PIN 34
#define VA_MAX_READ 100

#define BIG_FLAG_FIRST 0
#define BIG_FLAG_LAST  30

#define SMALL_FLAG_FIRST 31
#define SMALL_FLAG_LAST  45

#define RING_FIRST 46
#define RING_LAST  89

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

#define WIFI_SSID "Das Spinnennetz"
#define WIFI_PSK  "fqiCO7Sie3UoiR2vj0zB"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
AsyncWebServer server(80);
arduinoFFT FFT = arduinoFFT();


// Colors: 0xaabbggrr
uint32_t colors[LED_COUNT];

uint8_t brightness = 0xff;
uint8_t normalize = 0;

uint32_t single_color_all = 0;
uint32_t big_flag_color = 0;
uint32_t small_flag_color = 0;
uint32_t ring_color = 0;
uint8_t single_color_type = 0; // 0 -> all 1 -> seperate


// Functions
// lamp.ino
uint16_t rgb656(uint8_t r, uint8_t g, uint8_t b);
void update_leds();
uint32_t hsv_to_rgb(float h, float s, float v);
void clear();
// animations.ino
void start_animation(const char* name);
void run_animations();

void led_thread(void* args) {
  while (1) {
    run_animations();
    update_leds();
  }
}

TaskHandle_t LEDThreadHandle;

uint16_t va;

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(ILI9341_BLACK);

  //pinMode(AUDIO_PIN, INPUT);
  analogReadResolution(11);
  analogSetAttenuation(ADC_0db);

  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PSK);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.css", "text/css");
  });
  server.on("/logo.svg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logo.svg", "image/svg+xml");
  });
  server.on("/logo.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logo.png", "image/png");
  });
  server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery.min.js", "application/javascript");
  });
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });
  server.on("/antifa.svg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/antifa.svg", "image/svg+xml");
  });
  server.on("/antifa.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/antifa.png", "image/png");
  });

  server.on("/set_basic", HTTP_POST, [](AsyncWebServerRequest *request){
      if (request->hasParam("brightness", true)) {
          brightness = request->getParam("brightness", true)->value().toInt();
      }
      if (request->hasParam("normalized", true)) {
          String normalized = request->getParam("normalized", true)->value();
          if (normalized == "true") normalize = 1;
          if (normalized == "false") normalize = 0;
      }
      request->send(200, "text/plain", "\n");
  });

  // Animations
  server.on("/animation/single_color", HTTP_POST, [](AsyncWebServerRequest *request){
      if (request->hasParam("color", true)) {
          String rgb_string = request->getParam("color", true)->value();
          uint32_t rgb = strtol(rgb_string.c_str()+1,NULL,16);
          uint8_t r = (uint8_t)(rgb>>16);
          uint8_t g = (uint8_t)(rgb>>8);
          uint8_t b = (uint8_t)rgb;
          single_color_all = (b << 16) | (g << 8) | r;
          single_color_type = 0;
          start_animation("single_color");
      }
      request->send(200, "text/plain", "\n");
  });
  server.on("/animation/parts_color", HTTP_POST, [](AsyncWebServerRequest *request){
      String rgb_string;
      uint32_t rgb;
      uint8_t r, g, b;
      if (request->hasParam("ring_color", true)) {
          rgb_string = request->getParam("ring_color", true)->value();
          rgb = strtol(rgb_string.c_str()+1,NULL,16);
          r = (uint8_t)(rgb>>16);
          g = (uint8_t)(rgb>>8);
          b = (uint8_t)rgb;
          ring_color = (b << 16) | (g << 8) | r;
      }
      if (request->hasParam("small_flag_color", true)) {
          rgb_string = request->getParam("small_flag_color", true)->value();
          rgb = strtol(rgb_string.c_str()+1,NULL,16);
          r = (uint8_t)(rgb>>16);
          g = (uint8_t)(rgb>>8);
          b = (uint8_t)rgb;
          small_flag_color = (b << 16) | (g << 8) | r;
      }
      if (request->hasParam("big_flag_color", true)) {
          rgb_string = request->getParam("big_flag_color", true)->value();
          rgb = strtol(rgb_string.c_str()+1,NULL,16);
          r = (uint8_t)(rgb>>16);
          g = (uint8_t)(rgb>>8);
          b = (uint8_t)rgb;
          big_flag_color = (b << 16) | (g << 8) | r;
      }
      single_color_type = 1;
      start_animation("single_color");
      request->send(200, "text/plain", "\n");
  });
  server.on("/animation/off", HTTP_POST, [](AsyncWebServerRequest *request){
      start_animation("off");
      request->send(200, "text/plain", "\n");
  });
  server.on("/animation/rainbow", HTTP_POST, [](AsyncWebServerRequest *request){
      start_animation("rainbow");
      request->send(200, "text/plain", "\n");
  });
  server.on("/animation/rainbow_cycle", HTTP_POST, [](AsyncWebServerRequest *request){
      start_animation("rainbow_cycle");
      request->send(200, "text/plain", "\n");
  });
  server.on("/animation/rainbow_bars", HTTP_POST, [](AsyncWebServerRequest *request){
      start_animation("rainbow_bars");
      request->send(200, "text/plain", "\n");
  });
  server.on("/animation/audio_visual", HTTP_POST, [](AsyncWebServerRequest *request){
      start_animation("audio_visual");
      request->send(200, "text/plain", "\n");
  });

  server.begin();

  xTaskCreatePinnedToCore(
    led_thread,
    "led_thread",
    10000,
    NULL,
    0,
    &LEDThreadHandle,
    0
  );

  start_animation("off");

}

void loop() {
  delay(1);
}
