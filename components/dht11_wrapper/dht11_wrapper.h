#ifndef DHT11_WRAPPER_H
#define DHT11_WRAPPER_H

#include "driver/gpio.h"
#include "esp_err.h"

// This file wraps the DHT11 sensor library
// It simplifies reading temperature and humidity

// Structure to store sensor data
typedef struct {
    float humidity;        // humidity percentage
    float temperature_c;   // temperature in Celsius
} dht11_data_t;

// Initializes the DHT11 sensor on a given GPIO pin
void dht11_wrapper_init(gpio_num_t pin);

// Reads data from the DHT11 sensor
// Stores results in the provided struct
// Returns ESP_OK if successful
esp_err_t dht11_wrapper_read(dht11_data_t *out);

#endif