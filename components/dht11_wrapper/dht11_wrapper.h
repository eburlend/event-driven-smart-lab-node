#ifndef DHT11_WRAPPER_H
#define DHT11_WRAPPER_H

#include "driver/gpio.h"
#include "esp_err.h"

typedef struct {
    float humidity;
    float temperature_c;
} dht11_data_t;

void dht11_wrapper_init(gpio_num_t pin);
esp_err_t dht11_wrapper_read(dht11_data_t *out);

#endif