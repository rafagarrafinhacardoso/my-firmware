#include "Arduino.h"

// unsigned long startMillis;
// unsigned long currentMillis;

bool ledEstado = false;

String getMAC()
{
  char ssid[23];

  uint64_t chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
  uint16_t chip = (uint16_t)(chipid >> 32);

  snprintf(ssid, 23, "%04X%08X", chip, (uint32_t)chipid);
  // Serial.println(ssid);

  return String(ssid);
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == 'a') {
    Serial.println("true");
    digitalWrite(ledB, ledEstado);   // Turn the LED on (Note that LOW is the voltage level
    ledEstado = !ledEstado;
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    Serial.println("mensagem diferente");
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      String resp = getMAC();
      snprintf (msg, MSG_BUFFER_SIZE, "conect serialNumber: %s", resp);
      client.publish("status",msg);
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}