#pragma once

#include <Arduino.h>

void wifi_init();

bool set_wifi_credentials(const char* ssid, const char* psk);
void write_credentials_to_fs();
void load_credentials_from_fs();