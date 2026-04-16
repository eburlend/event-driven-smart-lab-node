#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <stdbool.h>

// Starts the MQTT client
void mqtt_app_start(void);

// Publishes a payload to the given topic
void mqtt_publish(const char *topic, const char *data);

// Returns current MQTT connection status
bool mqtt_is_connected(void);

#endif