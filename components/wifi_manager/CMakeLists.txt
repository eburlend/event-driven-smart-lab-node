# Build configuration for the wifi_manager component
# This component handles Wi-Fi initialization and connection logic

idf_component_register(
    SRCS "wifi_manager.c"                 # Source file for Wi-Fi logic
    INCLUDE_DIRS "."                      # Include directory for header files
    REQUIRES esp_wifi esp_event esp_netif # Required ESP-IDF Wi-Fi libraries
)