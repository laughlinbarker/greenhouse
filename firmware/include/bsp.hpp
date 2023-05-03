#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include <OneWire.h>
#include <DallasTemperature.h>

/**
 * @brief Initialize the board and periperals.
*/
namespace bsp
{
    void initialize_board();

    DallasTemperature& get_sensors();
    WiFiUDP& get_udp();
    WiFiClient& get_client();
}; // namespace bsp