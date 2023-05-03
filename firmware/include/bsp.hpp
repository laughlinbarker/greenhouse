#pragma once

#include <Arduino.h>
#include <WiFi.h>

#include <OneWire.h>
#include <DallasTemperature.h>

/**
 * @brief Initialize the board and periperals.
*/
namespace bsp
{
    void initialize_board();

    DallasTemperature& get_sensors();
}; // namespace bsp