//! Settings file
#include "settings.h"

//! Arduino libraries
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "MutichannelGasSensor.h"

//! Custom libraries
#include "wifi.h"

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

	sendToWifi(wifiSerial, "AT+CWMODE=2", TIMEOUT, DEBUG);
	sendToWifi(wifiSerial, "AT+CIFSR", TIMEOUT, DEBUG);
	sendToWifi(wifiSerial, "AT+CIPMUX=1", TIMEOUT, DEBUG);
	sendToWifi(wifiSerial, "AT+CIPSERVER=1,80", TIMEOUT, DEBUG);
	sendToUno("Wifi Connection is running", TIMEOUT, DEBUG);
}

void loop() {
	if (millis() - timer1 >= SENSOR_RATE) {
		float sensorPpm;

		float ppmReadings[7];
		float voltageReadings[2];

		Serial.println("Multichannel gas sensor readings");

		sensorPpm = gas.measure_NH3();
		Serial.print("NH3= ");
		if (sensorPpm >= 0)
			Serial.print(sensorPpm / 10e3);
		else
			Serial.print("invalid");
		Serial.println(" %");
		ppmReadings[0] = sensorPpm;

		sensorPpm = gas.measure_CO();
		Serial.print("CO= ");
		if (sensorPpm >= 0)
			Serial.print(sensorPpm / 10e3);
		else
			Serial.print("invalid");
		Serial.println(" %");
		ppmReadings[1] = sensorPpm;

		sensorPpm = gas.measure_NO2();
		Serial.print("NO2= ");
		if (sensorPpm >= 0)
			Serial.print(sensorPpm / 10e3);
		else
			Serial.print("invalid");
		Serial.println(" %");
		ppmReadings[2] = sensorPpm;

		sensorPpm = gas.measure_C3H8();
		Serial.print("C3H8= ");
		if (sensorPpm >= 0)
			Serial.print(sensorPpm / 10e3);
		else
			Serial.print("invalid");
		Serial.println(" %");
		ppmReadings[3] = sensorPpm;

		sensorPpm = gas.measure_C4H10();
		Serial.print("C4H10= ");
		if (sensorPpm >= 0)
			Serial.print(sensorPpm / 10e3);
		else
			Serial.print("invalid");
		Serial.println(" %");
		ppmReadings[4] = sensorPpm;

		sensorPpm = gas.measure_CH4();
		Serial.print("CH4= ");
		if (sensorPpm >= 0)
			Serial.print(sensorPpm / 10e3);
		else
			Serial.print("invalid");
		Serial.println(" %");
		ppmReadings[5] = sensorPpm;

		sensorPpm = gas.measure_H2();
		Serial.print("H2= ");
		if (sensorPpm >= 0)
			Serial.print(sensorPpm / 10e3);
		else
			Serial.print("invalid");
		Serial.println(" %");
		ppmReadings[6] = sensorPpm;

		//* Gas Leakage sensor
		Serial.println("\nMQ2 gas sensor readings");

		float sensorVoltage;

		sensorVoltage = analogRead(A0) / 1024 * 5.0;

		Serial.print("MQ2 reading = ");
		Serial.print(sensorVoltage);
		Serial.println("V");
		voltageReadings[0] = sensorVoltage;

		//* HCHO sensor
		sensorVoltage = analogRead(A1) / 1024 * 5.0;

		Serial.print("HCHO reading = ");
		Serial.print(sensorVoltage);
		Serial.println("V");
		voltageReadings[1] = sensorVoltage;

		timer2++;
		if (timer2 >= WIFI_RATE_MULTIPLIER) {
			timer2 = 0;

			char* str;

			for (int i = 0; i < 7; i++) {
				dtostrf(ppmReadings[i], 10, 4, str);
				sendData(wifiSerial, str, TIMEOUT, DEBUG);
			}
		}

		timer1 = millis();
	}
}