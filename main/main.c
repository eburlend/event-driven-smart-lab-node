#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "cJSON.h"

#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "dht11_wrapper.h"

static const char *TAG = "MAIN";

// DHT11 data pin
#define DHT11_DATA_GPIO GPIO_NUM_5

// Single telemetry topic for Grafana
#define MQTT_TOPIC_TELEMETRY "gcu/lab/env/telemetry"

void app_main(void)
{
    dht11_data_t reading;
    esp_err_t ret;

    ESP_LOGI(TAG, "Booting system...");

    // Initialize NVS
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Starting DHT11 MQTT Grafana node");

    // Initialize sensor
    dht11_wrapper_init(DHT11_DATA_GPIO);
    vTaskDelay(pdMS_TO_TICKS(2000));

    // Start Wi-Fi
    wifi_init_sta();

    // Wait for Wi-Fi connection
    while (!wifi_is_connected())
    {
        ESP_LOGI(TAG, "Waiting for Wi-Fi connection...");
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    ESP_LOGI(TAG, "Wi-Fi connected, starting MQTT");

    // Start MQTT
    mqtt_app_start();

    // Wait for MQTT connection
    while (!mqtt_is_connected())
    {
        ESP_LOGI(TAG, "Waiting for MQTT connection...");
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    while (1)
    {
        bool read_ok = false;

        // Retry sensor read a few times
        for (int attempt = 0; attempt < 3; attempt++)
        {
            if (dht11_wrapper_read(&reading) == ESP_OK)
            {
                read_ok = true;
                break;
            }

            ESP_LOGW(TAG, "DHT11 read failed, retry %d/3", attempt + 1);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }

        if (read_ok)
        {
            // Build one JSON message for Grafana
            cJSON *root = cJSON_CreateObject();

            if (root != NULL)
            {
                cJSON_AddStringToObject(root, "device", "esp32-01");
                cJSON_AddNumberToObject(root, "temperature", reading.temperature_c);
                cJSON_AddNumberToObject(root, "humidity", reading.humidity);

                char *json_payload = cJSON_PrintUnformatted(root);

                if (json_payload != NULL)
                {
                    ESP_LOGI(TAG, "Message to %s: %s", MQTT_TOPIC_TELEMETRY, json_payload);

                    mqtt_publish(MQTT_TOPIC_TELEMETRY, json_payload);

                    ESP_LOGI(TAG, "Published successfully.");

                    cJSON_free(json_payload);
                }
                else
                {
                    ESP_LOGW(TAG, "Failed to create JSON payload");
                }

                cJSON_Delete(root);
            }
            else
            {
                ESP_LOGW(TAG, "Failed to create JSON object");
            }
        }
        else
        {
            ESP_LOGW(TAG, "Failed to read DHT11 after retries");
        }

        // Publish every 5 seconds
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}