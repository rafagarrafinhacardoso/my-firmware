#include "Arduino.h"

bool ledEstado = false;

String getMAC()
{
  char ssid[23];

  uint64_t chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
  uint16_t chip = (uint16_t)(chipid >> 32);

  snprintf(ssid, 23, "%04X%08X", chip, (uint32_t)chipid);

  return String(ssid);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char sub[30];
  String myMac = getMAC();
  sniprintf(sub, 30, "device/%s/mensage", myMac);
  if (strcmp(sub, topic) == 0) // if ( sub == topic ) compara char ****
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
  // while (!client.connected()) {
  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect("arduinoClient"))
  {
    Serial.println("connected");
    // Once connected, publish an announcement...
    String myMac = getMAC();
    snprintf(msg, MSG_BUFFER_SIZE, "conect serialNumber: %s", myMac);
    client.publish("status", msg);
    // ... and resubscribe
    char sub[30];
    sniprintf(sub, 30, "device/%s/mensage", myMac);
    client.subscribe(sub);
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

void status()
{
  String serialNumber = getMAC();
  snprintf(msg, MSG_BUFFER_SIZE, "{\"serialNumber\":\"%s\",\"status\":\"ativo\"}", serialNumber); // "{\"trackerId\":%d,\"height\":%d}"
  snprintf(tpc, TPC_BUFFER_SIZE, "device/%s/status", serialNumber);
  Serial.println(msg);
  client.publish(tpc, msg);
}