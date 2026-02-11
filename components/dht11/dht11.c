// Elijah Burlend
// 01/30/26
// SWE-410
// ESP-32 and DHT-11
// In Class Activity

// Includes the public header for this module
#include "dht11.h"

// Includes ESP-IDF
#include "esp_log.h"

// Includes the DHT sensor library from esp-idf-lib
// This library manages communication with DHT-11 sensor
#include "dht.h"

//Define a constant string used to label log messages
// static -Limits the visibility of the variable to this file only
// TAG: ESP-IDF logging convention
// char * means pointer to characters
// * means it points to the first char
// DHT11_WRAPPER becomes the label in the serial monitor
static const char *TAG = "DHT11_WRAPPER"

// Stores the GPIO pin number used for data line
// Initialized to GPIO_NUM_NC (Not Connected) to indicate that
// dht11_init() has not been called yet
static gpio_num_t s_pin = GPIO_NUM_NC;
//Elijah Burlend


