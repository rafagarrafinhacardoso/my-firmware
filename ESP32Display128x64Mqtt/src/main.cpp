/*
Monitor de movimento moni-movi
*/
#include <Arduino.h>
#include <WiFiManager.h>
#include <PubSubClient.h> // #define MQTT_MAX_PACKET_SIZE 256

WiFiClient net;
PubSubClient client(net);

#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>

Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// #include <ArduinoJson.h>
// DynamicJsonDocument doc(16384);

#define btn 23

// unsigned long startMillis;
// unsigned long currentMillis;

#include "config.h"
#include "function.h"
void setup()
{
  WiFiManager wm;

  pinMode(btn, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Start");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.display();
  delay(500); // Pause for 2 seconds
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
  // delay(500); // Pause for 2 seconds
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Display MQTT 1.0");
  display.println("wifiConect \n-> WiFiManager");
  display.display();
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

  if (!mpu.begin())
  {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally

  Serial.println(getMAC());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Enviar mensagem para:");
  display.println("device/" + getMAC() + "/mensage");
  display.display();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
    display.println("Reconectando Cliente MQTT");
    display.display();
  }
  client.loop();

  if (!digitalRead(btn))
  {
    Serial.println("Btn precionado");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Limpa display");
    display.println("Enviar mensagem para:");
    display.println("device/" + getMAC() + "/mensage");
    display.display();

    // String serialNumber = getMAC();
    // snprintf(msg, MSG_BUFFER_SIZE, "{\"serialNumber\":\"%s\"}", serialNumber); // "{\"trackerId\":%d,\"height\":%d}"
    // snprintf(tpc, TPC_BUFFER_SIZE, "test");
    // Serial.println(msg);
    // client.publish(tpc, msg);
    while (!digitalRead(btn))
    {
      // display.clearDisplay();
      // display.setCursor(0, 0);
      // display.println("Leitura Finalizada");
      // display.println("Solte o botao");
      // display.display();
      delay(500);
    }
  }
}
