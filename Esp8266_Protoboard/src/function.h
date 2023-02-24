#include "Arduino.h"

bool ledEstado = false;

void getMAC(char *ret)
{
  // char ssid[23];

  uint64_t chipid = ESP.getChipId(); // The chip ID is essentially its MAC address(length: 6 bytes).
  uint16_t chip = (uint16_t)(chipid >> 32);

  snprintf(ret, 23, "%04X%08X", chip, (uint32_t)chipid);

  // return ssid;
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < (int)length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  // if ((char)payload[0] == 'a') {
  //   Serial.println("true");
  //   // but actually the LED is on; this is because
  //   // it is active low on the ESP-01)
  // } else {
  //   Serial.println("mensagem diferente");
  // }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // client.subscribe("inTopic");
      snprintf(tpc, TPC_BUFFER_SIZE, "device/%s/mensage", serialNumber);
      Serial.print("Client sub: ");
      Serial.println(tpc);
      client.subscribe(tpc);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void status_MQTT()
{
  // char serialNumber[32];
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisStatusMQTT >= 10000)
  {
    previousMillisStatusMQTT = currentMillis;
    getMAC(serialNumber);
    snprintf(msg, MSG_BUFFER_SIZE, "{\"serialNumber\":\"%s\",\"status\":\"ativo\"}", serialNumber); // "{\"trackerId\":%d,\"height\":%d}"
    snprintf(tpc, TPC_BUFFER_SIZE, "device/%s/status", serialNumber);
    // Serial.println(msg);
    client.publish(tpc, msg);
  }
}