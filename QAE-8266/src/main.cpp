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

HTTPClient client; //? Http client for POST requests
int sendHttpPost(String);

void setup() {
	//* Setup serial communication with the Uno
	Serial.begin(115200);
	Serial.println("ESP=poweron");

	//* Start wifi connection to the AP
	if (QAE_SECURITY_TYPE == "open")
		WiFi.begin(QAE_SSID);
	else
		WiFi.begin(QAE_SSID, QAE_PASSWORD);
	Serial.println("ESP=wificonnect");

	//* Error out until connected to the AP
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("ESP=nowifi");
	}
	Serial.println("ESP=wifiok");
	Serial.print("ESP=debug=IP is: ");
	//* Send local IP to the Uno for debug
	Serial.println(WiFi.localIP());

	while (1){
		if (Serial.available())
			if (Serial.readStringUntil('\n').equals("UNO=ok")) break;
	}
}

void loop() {
	if (Serial.available()) { //? Serial buffer receives data
		String received = Serial.readStringUntil('\n'); //? Frame as a string

		if (received.startsWith("UNO=")) { //? UNO= determines that it's the Uno that sent it
			received = received.substring(5);
			//? Determine the command used :
			if (received.startsWith("data=")) { //? data= determines that the uno sends in data
				Serial.println("ESP=dataok");
				float data[NUMERIC_VALUES_COUNT + ANALOG_VALUES_COUNT]; //* Buffer for the data

				received = received.substring(6);
				String substring;
				for (int i = 0; i < NUMERIC_VALUES_COUNT + ANALOG_VALUES_COUNT; i++) {
					byte index = received.indexOf(',');

					substring = received.substring(0, index);
					received = received.substring(index + 1);

					data[i] = substring.toFloat(); //* convert substring to float to be stored in buffer
				}

				//! JSON SERIALIZATION
				const size_t capacity = JSON_ARRAY_SIZE(NUMERIC_VALUES_COUNT + ANALOG_VALUES_COUNT) + JSON_OBJECT_SIZE(4);
				DynamicJsonDocument doc(capacity);

				doc["sender"] = QAE_SENDER; //* Sender
				doc["password"] = QAE_PASSWORD; //* Password / passcode for the API

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
				sendHttpPost(jsonOutput); //? Send to the API using HTTP POST request.

			}  // else if (received.startsWith("something")){} to add new commands
		}
	}
}

//? Send 'data' using HTTP POST to the defined API address. 
int sendHttpPost(String data) {
	client.begin(QAE_API_ADDRESS);
	client.addHeader("Content-Type", "application/json");
	int httpResponseCode = client.POST("aaaa");
	client.end();
	//? Debug to the Uno.
	Serial.print("ESP=post=");
	Serial.println(httpResponseCode);
}