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

#define DHT11_DATA_GPIO GPIO_NUM_5

void app_main(void)
{
    dht11_data_t reading;
    char temp_payload[32];
    char humidity_payload[32];
    esp_err_t ret;

    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Starting DHT11 MQTT node");

    dht11_wrapper_init(DHT11_DATA_GPIO);

    // Give sensor a little time after init
    vTaskDelay(pdMS_TO_TICKS(2000));

    wifi_init_sta();

    // Wait until Wi-Fi is really connected
    while (!wifi_is_connected()) {
        ESP_LOGI(TAG, "Waiting for Wi-Fi connection...");
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    ESP_LOGI(TAG, "Wi-Fi connected, starting MQTT");

    mqtt_app_start();

    // Give MQTT time to connect
    vTaskDelay(pdMS_TO_TICKS(3000));

    while (1) {
        bool read_ok = false;

        // Try a few times before giving up
        for (int attempt = 0; attempt < 3; attempt++) {
            if (dht11_wrapper_read(&reading) == ESP_OK) {
                read_ok = true;
                break;
            }

            ESP_LOGW(TAG, "DHT11 read failed, retry %d/3", attempt + 1);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }

        if (read_ok) {
            snprintf(temp_payload, sizeof(temp_payload), "%.1f", reading.temperature_c);
            snprintf(humidity_payload, sizeof(humidity_payload), "%.1f", reading.humidity);

            mqtt_publish("gcu/swe410/dht11/temperature", temp_payload);
            mqtt_publish("gcu/swe410/dht11/humidity", humidity_payload);

            ESP_LOGI(TAG, "Temp=%s C  Humidity=%s%%",
                     temp_payload, humidity_payload);
        } else {
            ESP_LOGW(TAG, "Failed to read DHT11 after retries");
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}