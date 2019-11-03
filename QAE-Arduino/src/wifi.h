#include <Arduino.h>
#include <SoftwareSerial.h>

String sendToWifi(SoftwareSerial wifiSerial, String command, const int timeout, boolean debug);

String readWifiSerialMessage(SoftwareSerial wifiSerial);

void sendData(SoftwareSerial wifiSerial, String str, int responseTime, boolean debug);

boolean find(String string, String value);

String sendToUno(String command, const int timeout, boolean debug);