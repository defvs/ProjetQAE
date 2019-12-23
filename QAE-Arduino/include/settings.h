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
#define HW_BAUDRATE 9600

//* false for same serial as USB (HardwareSerial); true for SoftwareSerial at pins WIFI_PINS
#define WIFI_SERIAL_TYPE false

#if WIFI_SERIAL_TYPE == true
    #define WIFI_BAUDRATE 9600
    #define WIFI_PINS [2, 3]
#endif

//* Time in milliseconds for sensor update
#define SENSOR_RATE 2000

#define NUMERIC_VALUES_COUNT 7
#define ANALOG_VALUES_COUNT 2
#define ANALOG_PINS {A0, A1}