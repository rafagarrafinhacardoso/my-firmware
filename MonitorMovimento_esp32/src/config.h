// #include <Arduino.h>
// const char *serverMqtt = "http://127.0.0.1:1880/"; http://127.0.0.1/ 192.168.0.196
const char *mqtt_server = "192.168.0.195";

#define timePortal 80 // time portal Wifi Manager
#define sizeV 15 // tamanho Array 

#define MSG_BUFFER_SIZE (1000)
#define TPC_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
char tpc[TPC_BUFFER_SIZE];
char state[25];

#define sizeCharVector 100
// float AccelX[sizeV];