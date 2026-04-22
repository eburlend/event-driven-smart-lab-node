#include "door_sensor_wrapper.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "door_sensor_wrapper";

// Store the GPIO pin used by the door sensor
static gpio_num_t s_door_pin = GPIO_NUM_NC;

// This flag is set inside the ISR when the sensor is triggered
static volatile bool s_interrupt_triggered = false;

// ISR function for the door sensor pin
static void IRAM_ATTR door_sensor_isr_handler(void *arg)
{
    s_interrupt_triggered = true;
}

void door_sensor_init(gpio_num_t pin)
{
    s_door_pin = pin;

    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_POSEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << s_door_pin),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE
    };

    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(s_door_pin, door_sensor_isr_handler, NULL));

    ESP_LOGI(TAG, "Door sensor initialized on GPIO %d", s_door_pin);
}

bool door_sensor_was_triggered(void)
{
    if (s_interrupt_triggered)
    {
        s_interrupt_triggered = false;
        return true;
    }

    return false;
}

int door_sensor_get_level(void)
{
    if (s_door_pin == GPIO_NUM_NC)
    {
        return -1;
    }

    return gpio_get_level(s_door_pin);
}