#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <stdbool.h>

// This file handles all Wi-Fi related functionality
// It keeps connection logic out of main.c

// Initializes Wi-Fi in station mode and starts connection
void wifi_init_sta(void);

// Returns true if the ESP32 is connected to Wi-Fi
// Used by main.c to wait before starting MQTT
bool wifi_is_connected(void);

#endif