#include <Arduino.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // #define MQTT_MAX_PACKET_SIZE 256

WiFiClient net;
PubSubClient client(net);

#define LED 2

// unsigned long startMillis;
unsigned long previousMillisStatusMQTT;

#include "config.h"
#include "function.h"

void setup() {
  WiFiManager wm;

  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  bool res;
  wm.setConfigPortalTimeout(timePortal); // 80
  res = wm.autoConnect("ESP32");

  if (!res)
  {
    Serial.println("Failed to WiFi connect");
  }
  else
  {
    Serial.println("WiFi connected versao 2");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    client.setBufferSize(MSG_BUFFER_SIZE);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("INTERNET CONECTADA!");
  }
  getMAC(serialNumber);
  Serial.print("serialNumber: ");
  Serial.println(serialNumber);
}

void loop() {
  if (!client.connected())
  {
    reconnect();
    Serial.println("Reconectando Cliente MQTT");
  }
  client.loop();
  status_MQTT();
  // digitalWrite(LED, HIGH);
  // delay(1000);
  // digitalWrite(LED, LOW);
  // delay(1000);
}