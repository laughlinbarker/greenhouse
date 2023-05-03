#include "bsp.hpp"

#include <WiFiUdp.h>
#include <string>

void setup() {
  bsp::initialize_board();

  bsp::get_sensors().begin();
}

void sendData(String thisData, WiFiClient& client, const IPAddress dest_ip, const uint16_t dest_port);

void loop() {
  bsp::get_sensors().requestTemperatures(); 
  float temperatureC = bsp::get_sensors().getTempCByIndex(0);
  float temperatureF = bsp::get_sensors().getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("C");
  Serial.print(temperatureF);
  Serial.println("F");

  std::string msg;
  msg = "Temperature: " + std::to_string(temperatureC) + " C\n";
  
  IPAddress broadCast = WiFi.localIP();
  broadCast[3] = 255;

  IPAddress greenhouse;
  greenhouse.fromString("192.168.0.28");

  bsp::get_udp().beginPacket(broadCast.toString().c_str(), 1234);
  bsp::get_udp().write((const uint8_t*)(msg.c_str()), msg.length());
  bsp::get_udp().endPacket();

  sendData(msg.c_str(), bsp::get_client(), greenhouse, 1234);

  delay(5000);
}

void sendData(String thisData, WiFiClient& client, const IPAddress dest_ip, const uint16_t dest_port) {
  // if there's a successful connection:
  client.flush();
  if (client.connect(dest_ip, dest_port)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
        /*echo -e $(printf '\\x01\\x00\\x%02x\\x00%s' $((1 + ${#m})) "$m") | nc $@ >/dev/null*/
    client.write((uint8_t)1);
    client.write((uint8_t)0);
    // imprimir len + 1
    //imprimir el string
    client.write((uint8_t)(thisData.length() + 1));
    client.write((uint8_t)0);

    // here's the actual content of the PUT request:
    client.print(thisData);
    client.flush();
    client.stop();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
}