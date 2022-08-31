// This file corrects intellisense mishaps
// It is a no-op for executable code

#pragma once

#ifdef __INTELLISENSE__

#define CONFIG_BROKER_URL "mqtt://mqtt.eclipse.org"
#define CONFIG_ESP_WIFI_SSID "SSID"
#define CONFIG_ESP_WIFI_PASSWORD "password"
#define CONFIG_ESP_WIFI_RETRY 5

// In intellisense mode, __linux__ is defined by default
// This leads to various system headers trying to include
// unavailable headers like 'linux/version.h'.
#undef __linux__

#endif
