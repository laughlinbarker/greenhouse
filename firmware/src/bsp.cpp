#include "_bsp_private.hpp"

#include "bsp.hpp"

const char ssid[] = WIFI_SSID;     // your network SSID (name)
const char pass[] = WIFI_PASSWORD; // your network password

OneWire oneWire(oneWire_bus_pin);
DallasTemperature sensors(&oneWire);

void bsp::initialize_board()
{
    Serial.begin(115200);

    init_wifi(ssid, pass);

    Serial.println("Board initialized.");
}

DallasTemperature& bsp::get_sensors()
{
    return sensors;
}

void init_wifi(const char* ssid, const char* pass)
{
    // Connect to the WiFi network
    WiFi.begin(ssid, pass);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}