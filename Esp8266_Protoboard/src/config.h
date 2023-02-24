const char *mqtt_server = "192.168.0.246"; //    192.168.0.246  rede_k-> 192.168.0.196

#define timePortal 80 // time portal Wifi Manager

#define MSG_BUFFER_SIZE (256)  // = sizeCharVector * 6 + json outras variaveis
#define TPC_BUFFER_SIZE (60)
char msg[MSG_BUFFER_SIZE];
char tpc[TPC_BUFFER_SIZE];
char serialNumber[32];



// float AccelX[sizeV];