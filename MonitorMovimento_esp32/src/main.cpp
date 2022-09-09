/*
Monitor de movimento moni-movi
*/
#include <Arduino.h>

// #include <ArduinoJson.h>
// #include <MQTT.h>
#include <WiFiManager.h>

#define ledB 23

// #include <MQTT.h>
// MQTTClient client;
// #include <ArduinoJson.h>
// DynamicJsonDocument doc();
// #include <WiFi.h>
#include <PubSubClient.h>

WiFiClient net;
PubSubClient client(net);

// const char *serverMqtt = "http://127.0.0.1:1880/"; http://127.0.0.1/ 192.168.0.196
const char *mqtt_server = "192.168.0.195";
// unsigned long startMillis;
// unsigned long currentMillis;

#define MSG_BUFFER_SIZE	(50)
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
  Serial.println(getMAC());
}

void loop()
{
  // if(client.connect("public", "public", "public")){
  //   Serial.print("c");
  //   Serial.print(client.subscribe("/test"));
  // }
  // if (!client.connected()) {
  //   reconnect();
  // }
  // client.loop();

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if (Serial.available())
  {
    String resposta = respostaMqtt();
    Serial.print("Enviando :");
    Serial.println(resposta);

    snprintf (msg, MSG_BUFFER_SIZE, "%s", resposta);

    client.publish("myTopic", msg);
    // if (client.connect("public", "public", "public"))
    // {
    //   Serial.print(client.publish("esp/mensagem", resposta));
    //   Serial.println(" resposta enviada");
    // }
  }
  digitalWrite(ledB, HIGH);
  delay(800);
  digitalWrite(ledB, LOW);
  delay(800);
  Serial.print(".");
}
