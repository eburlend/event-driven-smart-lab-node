# Build configuration for the mqtt_manager component
# This component manages MQTT broker connection and publishing

idf_component_register(
    SRCS "mqtt_manager.c"                 # Source file for MQTT logic
    INCLUDE_DIRS "."                      # Include directory for headers
    REQUIRES mqtt esp_event              # Required MQTT and event libraries
)