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

void loop()
{
	float c;

	Serial.println("Multichannel gas sensor readings");

	c = gas.measure_NH3();
	Serial.print("NH3= ");
	if (c >= 0)
		Serial.print(c/10e3);
	else
		Serial.print("invalid");
	Serial.println(" %");

	c = gas.measure_CO();
	Serial.print("CO= ");
	if (c >= 0)
		Serial.print(c/10e3);
	else
		Serial.print("invalid");
	Serial.println(" %");

	c = gas.measure_NO2();
	Serial.print("NO2= ");
	if (c >= 0)
		Serial.print(c/10e3);
	else
		Serial.print("invalid");
	Serial.println(" %");

	c = gas.measure_C3H8();
	Serial.print("C3H8= ");
	if (c >= 0)
		Serial.print(c/10e3);
	else
		Serial.print("invalid");
	Serial.println(" %");

	c = gas.measure_C4H10();
	Serial.print("C4H10= ");
	if (c >= 0)
		Serial.print(c/10e3);
	else
		Serial.print("invalid");
	Serial.println(" %");

	c = gas.measure_CH4();
	Serial.print("CH4= ");
	if (c >= 0)
		Serial.print(c/10e3);
	else
		Serial.print("invalid");
	Serial.println(" %");

	c = gas.measure_H2();
	Serial.print("H2= ");
	if (c >= 0)
		Serial.print(c/10e3);
	else
		Serial.print("invalid");
	Serial.println(" %");

	//* Gas Leakage sensor
	Serial.println("\nMQ2 gas sensor readings");

	float sensor_volt;
	float sensorValue;

	sensorValue = analogRead(A0);
	sensor_volt = sensorValue / 1024 * 5.0;

	Serial.print("MQ2 reading = ");
	Serial.print(sensor_volt);
	Serial.println("V");

	//* HCHO sensor
	sensorValue = analogRead(A1);
	sensor_volt = sensorValue / 1024 * 5.0;

	Serial.print("HCHO reading = ");
	Serial.print(sensor_volt);
	Serial.println("V");

	delay(3000);
}