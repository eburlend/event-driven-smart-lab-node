#include "mqtt_manager.h"

#include <stdbool.h>
#include "esp_log.h"
#include "mqtt_client.h"

// IP Address for the PI to connect to
#define MQTT_BROKER_URI "mqtt://192.168.50.1:1883"

// This is the MQTT username and password
#define MQTT_USERNAME   "mqttuser"
#define MQTT_PASSWORD   "RIPricky04!"

static const char *TAG = "mqtt_manager";

static esp_mqtt_client_handle_t client = NULL;
static bool mqtt_connected = false;

static void mqtt_event_handler(void *handler_args,
                               esp_event_base_t base,
                               int32_t event_id,
                               void *event_data)
{
    switch ((esp_mqtt_event_id_t) event_id) {

        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT connected");
            mqtt_connected = true;
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "MQTT disconnected");
            mqtt_connected = false;
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT event error");
            break;

        default:
            break;
    }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
        .credentials.username = MQTT_USERNAME,
        .credentials.authentication.password = MQTT_PASSWORD,
        // .session.keepalive = 60,
    };

    ESP_LOGI(TAG, "MQTT broker URI: %s", MQTT_BROKER_URI);

    client = esp_mqtt_client_init(&mqtt_cfg);

    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return;
    }

    ESP_ERROR_CHECK(esp_mqtt_client_register_event(
        client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL));
    ESP_ERROR_CHECK(esp_mqtt_client_start(client));

    ESP_LOGI(TAG, "MQTT started");
}

void mqtt_publish(const char *topic, const char *data)
{
    // Publish only if the client exists and the broker connection is active
    if (client != NULL && mqtt_connected) {
        esp_mqtt_client_publish(client, topic, data, 0, 1, 0);
    } else {
        ESP_LOGW(TAG, "Publish skipped - MQTT not connected");
    }
}

bool mqtt_is_connected(void)
{
    return mqtt_connected;
}