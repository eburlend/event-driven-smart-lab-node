#ifndef DOOR_SENSOR_WRAPPER_H
#define DOOR_SENSOR_WRAPPER_H

#include <stdbool.h>
#include "driver/gpio.h"

// This file wraps the door sensor interrupt logic
// It keeps GPIO interrupt handling out of main.c

// Initializes the door sensor GPIO pin and ISR
void door_sensor_init(gpio_num_t pin);

// Returns true one time when the door sensor interrupt has fired
// The flag is cleared after it is read
bool door_sensor_was_triggered(void);

// Returns the current raw GPIO level of the door sensor pin
int door_sensor_get_level(void);

#endif