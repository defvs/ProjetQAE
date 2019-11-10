//! ProjetQAE-Arduino (c) Daniel THIRION & Antonin ESTELLE
//! Licensed under LGPLv3. A copy of the license should have been given
//! with the software. If not, find it online.
//? -------------------------------------------------------------------
//? File Name: main.cpp
//? Author: Daniel THIRION
//? Co-author: Antonin ESTELLE
//? Supervision: M. CONTARET, IUT GEII Salon de Provence
//? Last modified: 3/11/2019
//? -------------------------------------------------------------------
//* Description
/* Part of ProjetQAE : Using Arduino, gas sensors, a RPi and a web server,
   Monitor and display the current air quality.
   More info at https://github.com/defvs/ProjetQAE
*/

//! Settings file
#include "settings.h"

//! Arduino libraries
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "MutichannelGasSensor.h"

SoftwareSerial wifiSerial = SoftwareSerial(2, 3);

unsigned long timer1 = millis();
byte timer2 = 0;

void setup() {
	//* USB Serial
	Serial.begin(9600);
	Serial.println("Starting...");

	//* Gas sensor power on
	gas.begin();
	gas.powerOn();

	//* WIFI Serial
	wifiSerial.begin(115200);
}

void loop() {
	if (Serial.available()){
		String received = wifiSerial.readStringUntil('\n');
		if (received.equals(F("ESP=wifiok")))
			wifiSerial.println("UNO=ok");
		if (received.startsWith(F("ESP=debug=")))
			Serial.println(received.substring(received.lastIndexOf('=')));
	}

	if (millis() - timer1 >= SENSOR_RATE) {  //? Every SENSOR_RATE milliseconds : Sensor update
		float sensorReading;				 //? Sensor reading buffer
		float ppmReadings[7];				 //? Digital readings buffer
		float voltageReadings[2];			 //? Voltage readings buffer

		//* NH3 Digital Readings (in PPM)
		sensorReading = gas.measure_NH3();
		ppmReadings[0] = sensorReading;
		if (DEBUG) {
			Serial.println(F("Multichannel gas sensor readings"));
			Serial.print(F("NH3= "));
			if (sensorReading >= 0)
				Serial.print(sensorReading / 10e3);
			else
				Serial.print(F("invalid"));
			Serial.println(F(" %"));
		}

		//* CO Digital Readings (in PPM)
		sensorReading = gas.measure_CO();
		ppmReadings[1] = sensorReading;
		if (DEBUG) {
			Serial.print("CO= ");
			if (sensorReading >= 0)
				Serial.print(sensorReading / 10e3);
			else
				Serial.print("invalid");
			Serial.println(" %");
		}

		//* NO2 Digital Readings (in PPM)
		sensorReading = gas.measure_NO2();
		if (DEBUG) {
			ppmReadings[2] = sensorReading;
			Serial.print("NO2= ");
			if (sensorReading >= 0)
				Serial.print(sensorReading / 10e3);
			else
				Serial.print("invalid");
			Serial.println(" %");
		}

		//* C3H8 Digital Readings (in PPM)
		sensorReading = gas.measure_C3H8();
		ppmReadings[3] = sensorReading;
		if (DEBUG) {
			Serial.print("C3H8= ");
			if (sensorReading >= 0)
				Serial.print(sensorReading / 10e3);
			else
				Serial.print("invalid");
			Serial.println(" %");
		}

		//* C4H10 Digital Readings (in PPM)
		sensorReading = gas.measure_C4H10();
		ppmReadings[4] = sensorReading;
		if (DEBUG) {
			Serial.print("C4H10= ");
			if (sensorReading >= 0)
				Serial.print(sensorReading / 10e3);
			else
				Serial.print("invalid");
			Serial.println(" %");
		}

		//* CH4 Digital Readings (in PPM)
		sensorReading = gas.measure_CH4();
		ppmReadings[5] = sensorReading;
		if (DEBUG) {
			Serial.print("CH4= ");
			if (sensorReading >= 0)
				Serial.print(sensorReading / 10e3);
			else
				Serial.print("invalid");
			Serial.println(" %");
		}

		//* H2 Digital Readings (in PPM)
		sensorReading = gas.measure_H2();
		ppmReadings[6] = sensorReading;
		if (DEBUG) {
			Serial.print("H2= ");
			if (sensorReading >= 0)
				Serial.print(sensorReading / 10e3);
			else
				Serial.print("invalid");
			Serial.println(" %");
		}

		//* MQ2 (Gas Leakage) sensor
		sensorReading = analogRead(A0) / 1024 * 5.0;
		voltageReadings[0] = sensorReading;
		if (DEBUG) {
			Serial.println("\nMQ2 gas sensor readings");
			Serial.print("MQ2 reading = ");
			Serial.print(sensorReading);
			Serial.println("V");
		}

		//* HCHO sensor
		sensorReading = analogRead(A1) / 1024 * 5.0;
		voltageReadings[1] = sensorReading;
		if (DEBUG) {
			Serial.print("HCHO reading = ");
			Serial.print(sensorReading);
			Serial.println("V");
		}

		//! Wifi Transmission
		timer2++;
		if (timer2 >= WIFI_RATE_MULTIPLIER) {  //? Every WIFI_RATE_MULTIPLIER iterations
			timer2 = 0;						   //? reset iteration count

			String output = "";

			for (int i = 0; i < 7; i++) {					//* digital readings
				
			}
			for (int i = 0; i < 1; i++) {					//* analog readings

			}
		}

		timer1 = millis();  //? Reset timer
	}
}