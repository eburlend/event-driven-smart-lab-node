// Standard I/O library
#include <stdio.h>

// Includes the core FreeRTOS definitions used by ESP-IDF
// FreeRTOS is the operatng system on the ESP32
#include "freertos/FreeRTOS.h"

// Include the task-related functions
// Gives us access to the vTaskDelay(), which pauses execution
#include "freertos/task.h"


// Include ESP-IDF logging macros
// This will print messages to serial monitor

#include "esp_log.h"

#include "dht11.h"


// Define a constant string  used to label log messages
static const char *TAG = "MAIN";

// Define which GPIO pin the DHT11 is connected to
#define DHT11_DATA_GPIO GPIO_NUM_4


//app_main() is the entry point for the ESP-IDF apps
void app_main(void)
{
    // Declare a var of type dht11_reading_t
    // This structure will hold humidity and temp data
    dht11_reading_t reading;

    // Logs an informational message the program has started
    ESP_LOGI(TAG, "Starting DHT11 example (GPIO %d)", (int)DHT11_DATA_GPIO);

    // Initialize the DHT11 driver
    // Pass in the pin
    dht11_init(DHT11_DATA_GPIO);

    // start an infinite loop
    // Embedded systems often run forever unless powered off or on
    while(1)
    {
        // Calls the DHT11 read function
        // The address of 'reading' is passed to the function
        esp_err_t err = dht11_read(&reading);

        // Checks whether the sensor read was successful
        if (err == ESP_OK)
        {
            // If successful, log the humidity and temp values
            // % .1f formats the float values to one decimal place
            ESP_LOGI(TAG, "Humidity: %.1f %% Temp: %.1f F (%.1f C)",
            reading.humidity, reading.temperature_f, reading.temperature_c);
        }
        else
        {
            // Pause the task to 2000 milliseconds (2 sec)
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
            vTaskDelay(pdMS_TO_TICKS(2000));

    }

}
