#!/bin/bash

# Check that WIFI_SSID and WIFI_PASSWORD are set
if [ -z "$WIFI_SSID" ] || [ -z "$WIFI_PASSWORD" ]; then
    echo "WIFI_SSID and WIFI_PASSWORD must be set"
    exit 1
fi

pio run -t upload && pio device monitor -b 115200