# ProjetQAE
## Environmental Air Quality control
## Using sensors, microcontrollers, and a REST API
### by Daniel THIRION and Antonin ESTELLE

# Components / Software
- Arduino Uno (ATMega328P)
- ESP-01 board (ESP8266)
- Raspberry Pi 3A as a webserver

Built using PlatformIO (coded in C++ and Arduino framework) for the microcontroller part, and Apache + PHP + MariaDB for the REST API.

The project was made using PlatformIO IDE on Visual Studio Code. To compile it, you can either use PIO Core and a CLI, or use PlatformIO IDE on VSCode or Atom. PIO will automatically download the board information, the framework, and the required libraries.
If you need to compile it using the Arduino IDE, you will need to move every .cpp, .h files from the directory ``src`` and ``include`` in the same directory, then download the required libraries using the IDE.

# Inner workings
## QAE-Arduino
The Uno is used to communicate with the digital sensors via I2C, or to the analog sensors using its ADC. It can also be used to process the data (averaging, calibrating, converting from sensor voltage to PPM...). It then sends its processed data over a software serial interface which will be picked up by the ESP8266.

The required libraries are as follows : MultichannelGasSensor by SeeedStudio;

## QAE-8266
Uses a ESP-01 board, which only has 8 pins. Only used for JSON creation from the data and sending of POST requests to the API.
Can be replaced by any ESP8266 board. The ESP-01 is small and compact over the other boards.

The required libraries are as follows : ESP8266 version of the framework Arduino.h; ESP8266HTTPClient; ESP8266WiFi;

## QAE-API
A simple REST API coded in PHP with MySQL for data storage. One PHP file accepts POST and GET requests;
### POST request: sending data to the API
The ESP8266 sends a JSON using an HTTP POST request. The PHP server picks it up and treats its data, storing it into the MySQL database. It returns different HTTP response codes depending on the result (``200 OK``, ``400 errored``, ``502 Internal Server Error``...)
The JSON is formed as follows :
```JSON
{
  data_numeric: [0, 0, 0, ... the numeric data, in PPM],
  data_analog: [12.43, 4.24, ... the analog data, in volts],
  sender: "esp1", (the sender's ID),
  password: "password" (low security password, defined in PHP; WARNING, not really secure.
}
```

### GET request: getting the data from the API
Any device can operate an HTTP GET request towards the same page. The API will respond with a JSON containing the data, and its latest update time as ``dd/mm/yyyy hh:mm:ss``. It is formated as follows :
```JSON

```

### MySQL Database
An example MySQL database is included with the program, as database.sql; it is a collection of SQL commands which will create and populate your database to match the example; you can then modify it using SQL queries or a frontend like PHPMyAdmin.

## QAE-Webpage
This is a frontend example of usage of the API. It periodically polls the API, and shows the data to the user as charts and averages.
It is made in HTML/CSS and Javascript (+ JQuery), and is completely independent of the values database as it is using the API with GET requests.
