#pragma once

#include <Arduino.h>


static constexpr int oneWire_bus_pin = 4;

// Initialize and connect WiFi to the network
// BLOCKING
void init_wifi(const char* ssid, const char* pass);
