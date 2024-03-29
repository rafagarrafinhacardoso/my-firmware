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

#define btn 23

// unsigned long startMillis;
// unsigned long currentMillis;
unsigned long previousMillisStatusMQTT;

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
  display.println("wifiConect");
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
  getMAC(serialNumber);
  Serial.println(serialNumber);
}

void loop()
{
  // display.clearDisplay();
  // display.setCursor(0, 0);

  if (!client.connected())
  {
    reconnect();
    display.println("Reconectando Cliente MQTT");
    display.display();
  }
  client.loop();
  status_MQTT();

  if (!digitalRead(btn))
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    Serial.println("Btn precionado");
    display.println("Iniciando leitura");
    display.display();
    String acce_x = "";
    String acce_y = "";
    String acce_z = "";
    String gyro_x = "";
    String gyro_y = "";
    String gyro_z = "";
    for (int x = 0; x < sizeV; x++)
    {
      sensors_event_t g, a, temp;
      mpu.getEvent(&g, &a, &temp);
      acce_x += a.acceleration.x;
      acce_y += a.acceleration.y;
      acce_z += a.acceleration.z;
      gyro_x += g.gyro.x;
      gyro_y += g.gyro.y;
      gyro_z += g.gyro.z;
      if (x < (sizeV - 1))
      {
        acce_x += ",";
        acce_y += ",";
        acce_z += ",";
        gyro_x += ",";
        gyro_y += ",";
        gyro_z += ",";
      }
      delay(temp_leitura);
    }
    Serial.println(acce_x);
    // String serialNumber = getMAC();
    char acceX[sizeCharVector];
    char acceY[sizeCharVector];
    char acceZ[sizeCharVector];
    char gyroX[sizeCharVector];
    char gyroY[sizeCharVector];
    char gyroZ[sizeCharVector];
    acce_x.toCharArray(acceX, sizeCharVector);
    acce_y.toCharArray(acceY, sizeCharVector);
    acce_z.toCharArray(acceZ, sizeCharVector);
    gyro_x.toCharArray(gyroX, sizeCharVector);
    gyro_y.toCharArray(gyroY, sizeCharVector);
    gyro_z.toCharArray(gyroZ, sizeCharVector);
    snprintf(msg, MSG_BUFFER_SIZE, "{\"serialNumber\":\"%s\",\"acceleration\":{\"x\":[%s],\"y\":[%s],\"z\":[%s]},\"gyro\":{\"x\":[%s],\"y\":[%s],\"z\":[%s]}}", serialNumber, acceX, acceY, acceZ, gyroX, gyroY, gyroZ); // "{\"trackerId\":%d,\"height\":%d}"
    snprintf(tpc, TPC_BUFFER_SIZE, "device/%s/movement", serialNumber);
    Serial.println(msg);
    client.publish(tpc, msg);
    while (!digitalRead(btn))
    {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Leitura Finalizada");
      display.println("Solte o botao");
      display.display();
      delay(500);
    }
    display.println();
    display.println("Enviar mensagem para:");
    snprintf(tpc, TPC_BUFFER_SIZE, "device/%s/mensage", serialNumber);
    display.println(tpc);
    display.display();
  }
}
