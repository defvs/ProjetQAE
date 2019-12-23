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
#include <Wire.h>
#include "MutichannelGasSensor.h"

#ifdef WIFI_BAUDRATE
	#include <SoftwareSerial.h>
	SoftwareSerial wifiSerial = SoftwareSerial(2, 3); // RX, TX
#else
	#define wifiSerial Serial
#endif

unsigned long timer1 = millis();

uint8_t analogPins[] = ANALOG_PINS;

void setup() {
	//* USB Serial
	Serial.begin(HW_BAUDRATE);
	Serial.println("Starting...");

	//* Gas sensor power on
	gas.begin();
	gas.powerOn();

	#ifdef WIFI_BAUDRATE
		//* WIFI Serial
		wifiSerial.begin(WIFI_BAUDRATE);
	#endif
}

void loop() {
	if (wifiSerial.available()){
		String received = wifiSerial.readStringUntil('\n');
		if (received.startsWith(F("ESP=wifiok"))){
			wifiSerial.println("UNO=ok");
			if (DEBUG) Serial.println("Uno connected to ESP");
		}
		if (received.startsWith(F("ESP=debug=")))
			Serial.println(received.substring(received.lastIndexOf('=') + 1));
	}

	if (millis() - timer1 >= SENSOR_RATE) {  //? Every SENSOR_RATE milliseconds : Sensor update
		float sensorReading;				 //? Sensor reading buffer
		float ppmReadings[7];				 //? Digital readings buffer
		float voltageReadings[ANALOG_VALUES_COUNT];			 //? Voltage readings buffer

		//* NH3 Digital Readings (in PPM)
		sensorReading = gas.measure_NH3();
		ppmReadings[0] = sensorReading;

		//* CO Digital Readings (in PPM)
		sensorReading = gas.measure_CO();
		ppmReadings[1] = sensorReading;

		//* NO2 Digital Readings (in PPM)
		sensorReading = gas.measure_NO2();

		//* C3H8 Digital Readings (in PPM)
		sensorReading = gas.measure_C3H8();
		ppmReadings[3] = sensorReading;

		//* C4H10 Digital Readings (in PPM)
		sensorReading = gas.measure_C4H10();
		ppmReadings[4] = sensorReading;

		//* CH4 Digital Readings (in PPM)
		sensorReading = gas.measure_CH4();
		ppmReadings[5] = sensorReading;

		//* H2 Digital Readings (in PPM)
		sensorReading = gas.measure_H2();
		ppmReadings[6] = sensorReading;

		for (byte i = 0; i < ANALOG_VALUES_COUNT; i++)
		{
			sensorReading = analogRead(analogPins[i]) / 1024 * 5.0;
			voltageReadings[i] = sensorReading;
		}

		//! Wifi Transmission
		String output = "UNO=data=";

		for (int i = 0; i < 7; i++) {					//* digital readings
			if (ppmReadings[i] > 0 &&  ppmReadings[i] < 1000000)
				output += String(ppmReadings[i]);
			else output += String(-1);
			output += ",";
		}
		for (int i = 0; i < 2; i++) {					//* analog readings
			output += String(voltageReadings[i]);
			output += ",";
		}

		if (DEBUG)
			Serial.println(output);
		wifiSerial.println(output);

		timer1 = millis();  //? Reset timer
	}
}