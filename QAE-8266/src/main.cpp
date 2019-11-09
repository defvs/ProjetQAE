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

HTTPClient client;
int sendHttpPost(String);

void setup() {
	Serial.begin(9600);
	Serial.println("ESP=poweron");

	if (QAE_SECURITY_TYPE == "open")
		WiFi.begin(QAE_SSID);
	else
		WiFi.begin(QAE_SSID, QAE_PASSWORD);
	Serial.println("ESP=wificonnect");

	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("ESP=nowifi");
	}
	Serial.println("ESP=wifiok");
	Serial.print("ESP=debug=IP is: ");
	Serial.println(WiFi.localIP());
}

void loop() {
	if (Serial.available()) {
		String received = Serial.readStringUntil('\n');

		if (received.startsWith("UNO=")) {
			received = received.substring(5);
			if (received.startsWith("data=")) {
				Serial.println("ESP=dataok");
				int data[7];

				received = received.substring(6);
				String substring;
				for (int i = 0; i < 7; i++) {
					byte index = received.indexOf(',');

					substring = received.substring(0, index);
					received = received.substring(index + 1);

					data[i] = substring.toInt();
				}

				const size_t capacity = JSON_ARRAY_SIZE(7) + JSON_OBJECT_SIZE(3);
				DynamicJsonDocument doc(capacity);

				doc["sender"] = "espmaison";
				doc["password"] = "password";

				JsonArray values_numeric = doc.createNestedArray("values_numeric");
				for (byte i = 0; i < 7; i++) {
					values_numeric.add(data[i]);
				}

				String jsonOutput;
				serializeJson(doc, jsonOutput);
				sendHttpPost(jsonOutput);

			}  // else if (received.startsWith("something")){} to add new commands
		}
	}
}

int sendHttpPost(String data) {
	client.begin(QAE_API_ADDRESS);
	client.addHeader("Content-Type", "application/json");
	int httpResponseCode = client.POST("aaaa");
	client.end();
	Serial.print("ESP=post=");
	Serial.println(httpResponseCode);
}