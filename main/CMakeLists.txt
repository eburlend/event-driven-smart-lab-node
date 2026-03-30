# This file defines how the main application is built
# It specifies the main source file and required components

idf_component_register(
    SRCS "main.c"                         # Main application source file
    INCLUDE_DIRS "."                      # Include current directory for headers
    REQUIRES wifi_manager mqtt_manager dht11_wrapper nvs_flash  # Required components
)