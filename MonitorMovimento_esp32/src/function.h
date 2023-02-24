#include "Arduino.h"

bool ledEstado = false;

void getMAC(char *ret)
{
  // char ssid[23];

  uint64_t chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
  uint16_t chip = (uint16_t)(chipid >> 32);

  snprintf(ret, 23, "%04X%08X", chip, (uint32_t)chipid);

  // return String(ssid);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  sniprintf(tpc, TPC_BUFFER_SIZE, "device/%s/mensage", serialNumber);
  if (strcmp(tpc, topic) == 0) // if ( sub == topic ) compara char ****
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    for (int i = 0; i < length; i++)
    {
      display.print((char)payload[i]);
    }
    display.display();
  }
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void reconnect()
{
  // Loop until we're reconnected
  // while (!client.connected()) {
  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect("arduinoClient"))
  {
    Serial.println("connected");
    // Once connected, publish an announcement...
    getMAC(serialNumber);
    snprintf(msg, MSG_BUFFER_SIZE, "conect serialNumber: %s", serialNumber);
    client.publish("status", msg);
    // ... and resubscribe
    // client.subscribe("inTopic");
    sniprintf(tpc, TPC_BUFFER_SIZE, "device/%s/mensage", serialNumber);
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
  // }
}

void status_MQTT()
{
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