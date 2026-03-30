#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_err.h"

#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "dht11_wrapper.h"

static const char *TAG = "MAIN";

// GPIO pin used for the DHT11 data line
#define DHT11_DATA_GPIO GPIO_NUM_5

void app_main(void)
{
    dht11_data_t reading;
    char temp_payload[32];
    char humidity_payload[32];
    esp_err_t ret;

    ESP_LOGI(TAG, "Booting...");

    // Initialize NVS
    // Required for Wi-Fi to work correctly
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Starting DHT11 MQTT node");

    // Initialize the DHT11 wrapper with the selected GPIO pin
    dht11_wrapper_init(DHT11_DATA_GPIO);

    // Small startup delay to allow the sensor to stabilize
    vTaskDelay(pdMS_TO_TICKS(2000));

    // Start Wi-Fi connection process
    wifi_init_sta();

    // Wait until Wi-Fi connected before continuing
    while (!wifi_is_connected()) {
        ESP_LOGI(TAG, "Waiting for Wi-Fi connection...");
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    ESP_LOGI(TAG, "Wi-Fi connected, starting MQTT");

    // Start MQTT after Wi-Fi is up
    mqtt_app_start();

    // Give MQTT time to connect to the broker
    vTaskDelay(pdMS_TO_TICKS(3000));

    while (1) {
        bool read_ok = false;

        // Try reading the DHT11 a few times before giving up
        // This helps with occasional timing-sensitive sensor failures
        for (int attempt = 0; attempt < 3; attempt++) {
            if (dht11_wrapper_read(&reading) == ESP_OK) {
                read_ok = true;
                break;
            }

            ESP_LOGW(TAG, "DHT11 read failed, retry %d/3", attempt + 1);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }

        if (read_ok) {
            // Convert sensor values into strings for MQTT publishing
            snprintf(temp_payload, sizeof(temp_payload), "%.1f", reading.temperature_c);
            snprintf(humidity_payload, sizeof(humidity_payload), "%.1f", reading.humidity);

            // Publish temperature and humidity to separate MQTT topics
            mqtt_publish("gcu/swe410/dht11/temperature", temp_payload);
            mqtt_publish("gcu/swe410/dht11/humidity", humidity_payload);

            // Log current values for verification in the serial monitor
            ESP_LOGI(TAG, "Temp=%s C  Humidity=%s%%",
                     temp_payload, humidity_payload);
        } else {
            ESP_LOGW(TAG, "Failed to read DHT11 after retries");
        }

        // Wait 5 seconds before the next reading
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}