//! ProjetQAE-8266 (c) Daniel THIRION & Antonin ESTELLE
//! Licensed under LGPLv3. A copy of the license should have been given
//! with the software. If not, find it online.
//? -------------------------------------------------------------------
//? File Name: settings.h
//? Author: Daniel THIRION
//? Co-author: Antonin ESTELLE
//? Supervision: M. CONTARET, IUT GEII Salon de Provence
//? Last modified: 3/11/2019
//? -------------------------------------------------------------------
//* Description
/*  Part of ProjetQAE : Using Arduino, gas sensors, a RPi and a web server,
	Monitor and display the current air quality.
	More info at https://github.com/defvs/ProjetQAE

	This file is the ESP8266 part of the project.
    Here are the settings for Wifi & API connection.
*/
#define DEBUG true
#define WIFI_BAUDRATE 9600

#define QAE_SSID "rpi"
#define QAE_SECURITY_TYPE "open"
#define QAE_PASSWORD ""

#define NUMERIC_VALUES_COUNT 7
#define ANALOG_VALUES_COUNT 2

#define API_ADDRESS "http://192.168.43.58"
#define API_SENDER "espmaison"
#define API_PASSWORD "password"