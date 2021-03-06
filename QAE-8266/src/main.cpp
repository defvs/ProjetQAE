//! ProjetQAE-8266 (c) Daniel THIRION & Antonin ESTELLE
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
/*  Part of ProjetQAE : Using Arduino, gas sensors, a RPi and a web server,
	Monitor and display the current air quality.
	More info at https://github.com/defvs/ProjetQAE

	This file is the ESP8266 part of the project. The Espressif module serves
	as a wifi bridge to send data that comes through serial to the online API
	via HTTP POST requests.
*/
#include "settings.h"

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include <ArduinoJson.h>

HTTPClient client;  //? Http client for POST requests
int sendHttpPost(String);

unsigned long timer1 = 0;

void setup() {
	//* Setup serial communication with the Uno
	Serial.begin(WIFI_BAUDRATE);
	Serial.println();
	Serial.println(F("ESP=poweron"));

	//* Start wifi connection to the AP
	if (QAE_SECURITY_TYPE == "open")
		WiFi.begin(QAE_SSID);
	else
		WiFi.begin(QAE_SSID, QAE_PASSWORD);
	Serial.println(F("ESP=wificonnect"));

	//* Error out until connected to the AP
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println(F("ESP=nowifi"));
	}
	Serial.println(F("ESP=wifiok"));
	if (DEBUG) {
		Serial.print(F("ESP=debug=IP is: "));
		//* Send local IP to the Uno for debug
		Serial.println(WiFi.localIP());
	}

	while (1) {
		if (millis() - timer1 >= 1000) {
			Serial.println(F("ESP=wifiok"));
		}
		if (Serial.available())
			if (Serial.readStringUntil('\n').startsWith("UNO=ok")) break;
	}

	Serial.println(F("ESP=ready"));
}

void loop() {
	if (Serial.available()) {							 //? Serial buffer receives data
		String received = Serial.readStringUntil('\n');  //? Frame as a string

		if (received.startsWith("UNO=")) {  //? UNO= determines that it's the Uno that sent it
			received = received.substring(4);
			if (DEBUG) Serial.println(received);
			//? Determine the command used :
			if (received.startsWith("data=")) {  //? data= determines that the uno sends in data
				Serial.println(F("ESP=dataok"));
				float data[NUMERIC_VALUES_COUNT + ANALOG_VALUES_COUNT];  //* Buffer for the data

				received = received.substring(5);
				if (DEBUG) Serial.println(received);

				String substring;
				for (int i = 0; i < NUMERIC_VALUES_COUNT + ANALOG_VALUES_COUNT; i++) {
					byte index = received.indexOf(',');

					substring = received.substring(0, index);
					received = received.substring(index + 1);

					data[i] = substring.toFloat();  //* convert substring to float to be stored in buffer
					if (DEBUG) Serial.println(data[i]);
				}

				//! JSON SERIALIZATION
				const size_t capacity = JSON_ARRAY_SIZE(NUMERIC_VALUES_COUNT + ANALOG_VALUES_COUNT) + JSON_OBJECT_SIZE(4);
				DynamicJsonDocument doc(capacity);

				doc["sender"] = API_SENDER;		 //* Sender
				doc["password"] = API_PASSWORD;  //* Password / passcode for the API

				//* Values in an array
				JsonArray values_numeric = doc.createNestedArray("values_numeric");
				for (byte i = 0; i < NUMERIC_VALUES_COUNT; i++) {
					values_numeric.add(data[i]);
				}
				JsonArray values_analog = doc.createNestedArray("values_analog");
				for (byte i = NUMERIC_VALUES_COUNT; i < NUMERIC_VALUES_COUNT + ANALOG_VALUES_COUNT; i++) {
					values_analog.add(data[i]);
				}

				String jsonOutput;
				serializeJson(doc, jsonOutput);
				sendHttpPost(jsonOutput);  //? Send to the API using HTTP POST request.

			} else if (received.startsWith("ping")) {
				Serial.println("ESP=pong");
			} else if (received.startsWith("network")) {
				Serial.print("ESP=network=");
				Serial.println(WiFi.localIP());
			}  // else if (received.startsWith("something")){} to add new commands
		}
	}
}

//? Send 'data' using HTTP POST to the defined API address.
int sendHttpPost(String data) {
	client.begin(API_ADDRESS);
	client.addHeader(F("Content-Type"), F("application/json"));
	int httpResponseCode = client.POST(data);
	client.end();
	//? Debug to the Uno.
	Serial.print("ESP=debug=HTTP POST response ");
	Serial.println(httpResponseCode);

	return httpResponseCode;
}