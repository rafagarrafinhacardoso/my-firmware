/*
Monitor de movimento moni-movi
*/
#include <Arduino.h>
// #include <ArduinoJson.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

WiFiClient net;
PubSubClient client(net);

#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

#define ledB 23

// const char *serverMqtt = "http://127.0.0.1:1880/"; http://127.0.0.1/ 192.168.0.196
const char *mqtt_server = "192.168.0.195";
// unsigned long startMillis;
// unsigned long currentMillis;

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

#include "function.h"
void setup()
{
  WiFiManager wm;

  pinMode(ledB, OUTPUT);
  digitalWrite(ledB, LOW);
  Serial.begin(115200);
  Serial.println("Start");

  bool res;
  wm.setConfigPortalTimeout(80); // Ver Config.h
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

  Serial.println(getMAC());
}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  // if (client.connect("public", "public", "public"))
  // {
  //   client.subscribe("comando/led");
  // }
  
  sensors_event_t g, a, temp;
  mpu.getEvent(&g, &a, &temp);

  display.clearDisplay();
  display.setCursor(0, 0);

  display.println("Accelerometer - m/s^2");
  display.print(a.acceleration.x, 1);
  display.print(", ");
  display.print(a.acceleration.y, 1);
  display.print(", ");
  display.print(a.acceleration.z, 1);
  display.println("");

  display.println("Gyroscope - rps");
  display.print(g.gyro.x, 1);
  display.print(", ");
  display.print(g.gyro.y, 1);
  display.print(", ");
  display.print(g.gyro.z, 1);
  display.println("");

  display.display();
  delay(500);
}
