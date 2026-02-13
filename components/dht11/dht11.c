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
static const char *TAG = "DHT11_WRAPPER";

// Stores the GPIO pin number used for data line
// Initialized to GPIO_NUM_NC (Not Connected) to indicate that
// dht11_init() has not been called yet
static gpio_num_t s_pin = GPIO_NUM_NC;


// Function
// Convert a temperature value from Celsius to Fahrenheit
// Declare static so it is only visible in this source file
static float c_to_f(float tempc)
{
    return(tempc * 9.0f / 5.0f) + 32.0f;
}

// Initialize the DHT11 driver wrapper
// This function records which GPIO pin the DHT11 line is 
// connected to
void dht11_init(gpio_num_t pin)
{
    // Store the provided GPIO pin in a static variable
    s_pin = pin;
    // %d: insert and integer here

    ESP_LOGI(TAG, "Initialized DHT11 on GPIO %d", (int)s_pin);
}

// Reads humidity and temperature
// The results are written into the structure pointed by *out
esp_err_t dht11_read(dht11_reading_t *out)
{
    // Check if dht11_int() was called
    // If not, the GPIO is still marked as "not connected"
    if(s_pin == GPIO_NUM_NC)
    {
        // Log the data and return fail
        ESP_LOGE(TAG, "dht11_inint() not called");
        return ESP_FAIL;
    }

    // Validate the output pointer
    // If it is NULL, we cannot safely write data to it
    if(out == NULL)
    {
        return ESP_ERR_INVALID_ARG;

    }

    // Local vars to store the raw sensor readings
    // f tells the compiler: float and not a double
    float humidity =  0.0f;
    float temperature_c = 0.0f;

    // Call the DHT library to read humidity and temp
    esp_err_t err = dht_read_float_data(
        DHT_TYPE_DHT11,              // Specifies the sensor
        s_pin,                       // GPIO pin
        &humidity,                   // Address where humidity will be stored
        &temperature_c               // Address where temp (degrees C)
        
    );

    //check if the library call was successful
    if(err != ESP_OK)
    {
        // Log an error
        ESP_LOGE(TAG, "dht_read_float_data failed");
        return err;
    }

    // Store the humidity reading output structure
    out->humidity = humidity;
    // Store the temp in Celsius
    out->temperature_c = temperature_c;
    // Convert the temp to Fahrenheit
    out->temperature_f = c_to_f(temperature_c);


    // Indicate that the read was successful
    return ESP_OK;

    








}