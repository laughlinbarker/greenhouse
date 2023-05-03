#include "bsp.hpp"

#include <WiFiClient.h>
#include <WiFiUdp.h>

#include <ArduinoJson.h>

#include <string>

WiFiUDP udp;
WiFiClient tcp_client;

#define MAX_NUM_SENSORS 10

struct sensor_data
{
  float temps[MAX_NUM_SENSORS];
  size_t num_temps;
  float humidity;
  float lux;
};

void send_data(String thisData, WiFiClient& client, const IPAddress dest_ip, const uint16_t dest_port);
void get_sensor_data(sensor_data &data);
void list_devices();
void print_address(DeviceAddress deviceAddress);

void setup()
{
  bsp::initialize_board();

  bsp::get_sensors().begin();
}

void loop() 
{
  list_devices();
  sensor_data data;

  get_sensor_data(data);

  DynamicJsonDocument doc(1024);

  doc["location"] = "greenhouse";
  doc["inside_air_temp"] = data.temps[0];
  doc["outside_air_temp"] = data.temps[1];
  doc["soil_temp"] = data.temps[2];
  doc["humidity"] = data.humidity;
  doc["lux"] = data.lux;

  String msg = doc.as<String>();
  
  IPAddress broadcast_ip = WiFi.localIP();
  broadcast_ip[3] = 255;

  IPAddress greenhouse_server_ip;
  greenhouse_server_ip.fromString("192.168.0.28");
  const uint16_t greenhouse_port = 7753;

  udp.beginPacket(broadcast_ip.toString().c_str(), greenhouse_port);
  udp.write((const uint8_t*)(msg.c_str()), msg.length());
  udp.endPacket();

  send_data(msg.c_str(), tcp_client, greenhouse_server_ip, greenhouse_port);

  delay(5000);
}

//Based on https://github.com/dicoy-zz/zeromq-arduino-example
void send_data(String thisData, WiFiClient& client, const IPAddress dest_ip, const uint16_t dest_port)
{
  client.flush();
  if (client.connect(dest_ip, dest_port)) {
    client.write((uint8_t)1);
    client.write((uint8_t)0);
    client.write((uint8_t)(thisData.length() + 1));
    client.write((uint8_t)0);
    client.print(thisData);
    client.flush();
    client.stop();
  } 
  else {
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
}

void get_sensor_data(sensor_data &data)
{
  for (size_t i = 0; i < MAX_NUM_SENSORS; i++)
  {
    data.temps[i] = NAN;
  }
  data.humidity = NAN;
  data.lux = NAN;
  bsp::get_sensors().requestTemperatures();
  data.num_temps = bsp::get_sensors().getDeviceCount();
  for (size_t i = 0; i < data.num_temps; i++)
  {
    data.temps[i] = bsp::get_sensors().getTempCByIndex(i);
  }
}

void list_devices()
{
  DeviceAddress tempDeviceAddress;
  // Grab a count of devices on the wire
  int numberOfDevices = bsp::get_sensors().getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++) {
    // Search the wire for address
    if(bsp::get_sensors().getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      print_address(tempDeviceAddress);
      Serial.println();
		} else {
		  Serial.print("Found ghost device at ");
		  Serial.print(i, DEC);
		  Serial.print(" but could not detect address. Check power and cabling");
		}
  }
}

void print_address(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}