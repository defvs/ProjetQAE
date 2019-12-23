//! ProjetQAE-Arduino (c) Daniel THIRION & Antonin ESTELLE
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
/* Part of ProjetQAE : Using Arduino, gas sensors, a RPi and a web server,
   Monitor and display the current air quality.
   More info at https://github.com/defvs/ProjetQAE

   This file contains settings which will be embedded into the Arduino.
   These settings cannot be changed after the compilation and upload.
*/

#define DEBUG true
#define USB_BAUDRATE 9600
#define WIFI_BAUDRATE 9600

//* Time in milliseconds for sensor update
#define SENSOR_RATE 2000
//* Send to Wifi every x SENSOR_RATE
#define WIFI_RATE_MULTIPLIER 1