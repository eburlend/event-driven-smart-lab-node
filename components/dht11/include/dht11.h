// #ifndef stands for "if not defined"
// prevents this header file from being included more than once
// during compiling.
// DHT11_H is a preprocessor macro name

#ifndef DHT11_H

// Defines the macro for the first time
#define DHT11_H

// Includes ESP-IDF's GPIO definitions
#include "driver/gpio.h"

// LThis lets our functions return meaningful error values
#include "esp_err.h"

// Defines a structure to hold a complete DHT11 sensor reading
// This groups related data together so we can return
// everything in single, clean object
typedef struct dht11
{
    // Relative humidity reading in percent (%RH)
    float humidity;
    // Temperature reading in Celsius (°C)
    float temperature_c;
    // Temperature reading degrees Fahrenheit (°F)
    float temperature_f;
} dht11_reading_t;
// dht11_reading_t: custom data type that groups sensor values
// into a single object (one complete reading).

// initializes the DHT11 driver wrapper
// this function stores which GPIO pin the DHT11 Data line
// is connected to.
void dht11_init(gpio_num_t pin);


// Read Humidity + temp (C and F)
// Syntax: esp_err_t - the return type defined by ESP-IDF
// Returns ESP_OK on success
// *out - pointer to the struct
esp_err_t dht11_read(dht11_reading_t *out);

// end of the #ifndef DHT_H
#endif // DHT11_H
