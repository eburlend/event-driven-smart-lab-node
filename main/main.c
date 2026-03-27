#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "dht11_wrapper.h"

static const char *TAG = "MAIN";

#define DHT11_DATA_GPIO GPIO_NUM_4

void app_main(void)
{
    dht11_data_t reading;
    char temp_payload[32];
    char humidity_payload[32];

    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_LOGI(TAG, "Starting DHT11 MQTT node");

    dht11_wrapper_init(DHT11_DATA_GPIO);

    wifi_init_sta();

    vTaskDelay(pdMS_TO_TICKS(3000));

    mqtt_app_start();

    vTaskDelay(pdMS_TO_TICKS(3000));

    while (1) {

        if (dht11_wrapper_read(&reading) == ESP_OK) {

            snprintf(temp_payload, sizeof(temp_payload), "%.1f", reading.temperature_c);
            snprintf(humidity_payload, sizeof(humidity_payload), "%.1f", reading.humidity);

            mqtt_publish("gcu/swe410/dht11/temperature", temp_payload);
            mqtt_publish("gcu/swe410/dht11/humidity", humidity_payload);

            ESP_LOGI(TAG, "Temp=%s C  Humidity=%s%%",
                     temp_payload, humidity_payload);
        }
        else {
            ESP_LOGW(TAG, "Failed to read DHT11");
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}