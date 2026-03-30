#include "dht11_wrapper.h"
#include "dht.h"

// Store the GPIO pin selected for the DHT11 data line
static gpio_num_t s_pin = GPIO_NUM_NC;

void dht11_wrapper_init(gpio_num_t pin)
{
    s_pin = pin;
}

esp_err_t dht11_wrapper_read(dht11_data_t *out)
{
    // Validate that the output pointer is valid
    // and that the sensor pin has been initialized
    if (out == NULL || s_pin == GPIO_NUM_NC) {
        return ESP_ERR_INVALID_ARG;
    }

    int16_t temperature = 0;
    int16_t humidity = 0;

    // Read data from the DHT11 sensor
    esp_err_t err = dht_read_data(DHT_TYPE_DHT11, s_pin, &humidity, &temperature);
    if (err != ESP_OK) {
        return err;
    }

    // Store the values in the output structure
    out->humidity = (float)humidity;
    out->temperature_c = (float)temperature;

    return ESP_OK;
}