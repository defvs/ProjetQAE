//! ProjetQAE-Arduino (c) Daniel THIRION & Antonin ESTELLE
//! Licensed under LGPLv3. A copy of the license should have been given
//! with the software. If not, find it online.
//? -------------------------------------------------------------------
//? File Name: wifi.h
//? Author: Daniel THIRION
//? Co-author: Antonin ESTELLE
//? Supervision: M. CONTARET, IUT GEII Salon de Provence
//? Last modified: 3/11/2019
//? -------------------------------------------------------------------
//* Description
/* Part of ProjetQAE : Using Arduino, gas sensors, a RPi and a web server,
   Monitor and display the current air quality.
   More info at https://github.com/defvs/ProjetQAE

   This file contains the headers for the wifi helper functions.
*/

#include <Arduino.h>
#include <SoftwareSerial.h>

String sendToWifi(SoftwareSerial wifiSerial, String command, const int timeout, boolean debug);

String readWifiSerialMessage(SoftwareSerial wifiSerial);

void sendData(SoftwareSerial wifiSerial, String str, int responseTime, boolean debug);

boolean find(String string, String value);

String sendToUno(String command, const int timeout, boolean debug);