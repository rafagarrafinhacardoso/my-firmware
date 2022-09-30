// #include <Arduino.h>
// const char *serverMqtt = "http://127.0.0.1:1880/"; http://127.0.0.1/ 192.168.0.196
const char *mqtt_server = "192.168.0.195"; //"192.168.43.51"; // http://192.168.43.51:3000    192.168.0.195  172.17.0.1

#define timePortal 80 // time portal Wifi Manager
#define temp_leitura 10
#define sizeV 160 // tamanho Array 
#define sizeCharVector 1000 // tamanho char Array numeros separado por , ("-0.00,") X6

#define MSG_BUFFER_SIZE (6000)  // = sizeCharVector * 6 + json outras variaveis
#define TPC_BUFFER_SIZE (60)
char msg[MSG_BUFFER_SIZE];
char tpc[TPC_BUFFER_SIZE];
char state[25];


// float AccelX[sizeV];