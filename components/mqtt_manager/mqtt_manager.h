#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <stdbool.h>

void mqtt_app_start(void);
void mqtt_publish(const char *topic, const char *data);
bool mqtt_is_connected(void);

#endif