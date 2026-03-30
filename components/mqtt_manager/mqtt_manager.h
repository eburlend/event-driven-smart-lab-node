#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <stdbool.h>

// This file handles all MQTT functionality
// It manages the connection to the broker and message publishing

// Initializes the MQTT client and connects to the broker
// Should be called after Wi-Fi is connected
void mqtt_app_start(void);

// Publishes data to a given MQTT topic
// topic = where the data is sent
// data = message payload (string)
void mqtt_publish(const char *topic, const char *data);

// Returns true if connected to the MQTT broker
bool mqtt_is_connected(void);

#endif