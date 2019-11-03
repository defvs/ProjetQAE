//! ProjetQAE-Arduino (c) Daniel THIRION & Antonin ESTELLE
//! Licensed under LGPLv3. A copy of the license should have been given
//! with the software. If not, find it online.
//? -------------------------------------------------------------------
//? File Name: wifi.cpp
//? Author: Daniel THIRION
//? Co-author: Antonin ESTELLE
//? Supervision: M. CONTARET, IUT GEII Salon de Provence
//? Last modified: 3/11/2019
//? -------------------------------------------------------------------
//* Description
/* Part of ProjetQAE : Using Arduino, gas sensors, a RPi and a web server,
  Monitor and display the current air quality.
  More info at https://github.com/defvs/ProjetQAE

  This file contains the wifi (ESP8266) helper functions.
*/

#include "wifi.h"

/*
* Name: readWifiSerialMessage
* Description: Function used to read data from ESP8266 Serial.
* Params: 
* Returns: The response from the esp8266 (if there is a reponse)
*/
String readWifiSerialMessage(SoftwareSerial wifiSerial)
{
  char value[100];
  int index_count = 0;
  while (wifiSerial.available() > 0)
  {
    value[index_count] = wifiSerial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}

/*
* Name: sendToWifi
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToWifi(SoftwareSerial wifiSerial, String command, const int timeout, boolean debug)
{
  String response = "";
  wifiSerial.println(command); // send the read character to the esp8266
  while ((millis() + timeout) > millis())
  {
    while (wifiSerial.available())
    {
      // The esp has data so display its output to the serial window
      char c = wifiSerial.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.println(response);
  }
  return response;
}

/*
* Name: sendToUno
* Description: Function used to send data to Arduino.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToUno(String command, const int timeout, boolean debug)
{
  String response = "";
  Serial.println(command); // send the read character to the esp8266
  long int time = millis();
  while ((time + timeout) > millis())
  {
    while (Serial.available())
    {
      // The esp has data so display its output to the serial window
      char c = Serial.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.println(response);
  }
  return response;
}

/*
* Name: sendData
* Description: Function used to send string to tcp client using cipsend
* Params: 
* Returns: void
*/
void sendData(SoftwareSerial wifiSerial, String str, int responseTime, boolean debug)
{
  String len = "";
  len += str.length();
  sendToWifi(wifiSerial, "AT+CIPSEND=0," + len, responseTime, debug);
  delay(100);
  sendToWifi(wifiSerial, str, responseTime, debug);
  delay(100);
  sendToWifi(wifiSerial, "AT+CIPCLOSE=5", responseTime, debug);
}

/*
* Name: find
* Description: Function used to match two string
* Params: 
* Returns: true if match else false
*/
boolean find(String string, String value)
{
  return string.indexOf(value) >= 0;
}