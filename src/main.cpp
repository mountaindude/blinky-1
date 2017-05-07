
/*************************************************************
breathe.ino
SparkFun SX1509 I/O Expander Example: breathe output
Jim Lindblom @ SparkFun Electronics
Original Creation Date: September 21, 2015
https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

This example demonstrates the SX1509's set-it-and-forget-it
breathe function. The SX1509 will pulse an LED, smoothly
ramping its brightness up-then-down. We'll set the pin up as
an ANALOG_OUTPUT, and call io.breathe() all in setup(), then
watch the LED pulse by itself in loop().

Hardware Hookup:
	SX1509 Breakout ------ Arduino -------- Breadboard
	      GND -------------- GND
	      3V3 -------------- 3.3V
		  SDA ------------ SDA (A4)
		  SCL ------------ SCL (A5)
		  15 --------------------------------- LED+
		                                 LED- -/\/\/\- GND
                                                330

Development environment specifics:
	IDE: Arduino 1.6.5
	Hardware Platform: Arduino Uno
	SX1509 Breakout Version: v2.0

This code is beerware; if you see me (or any other SparkFun
employee) at the local, and you've found our code helpful,
please buy us a round!

Distributed as-is; no warranty is given.
*************************************************************/

#include <Arduino.h>
#include <main.h>
#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h> // Include SX1509 library
#include <CMMC_OTA.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"



CMMC_OTA ota;


WiFiClient espClient;
PubSubClient client(espClient);

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS_A = 0x3E;  // 1st SX1509 I2C address
const byte SX1509_ADDRESS_B = 0x3F;  // 2nd SX1509 I2C address
SX1509 io_A, io_B; // Create an SX1509 object to be used throughout

// SX1509 Pin definition:
const byte SX1509_LED_PIN = 4; // LED to SX1509's pin 15

void setup()
{

    Serial.begin(115200);
    Serial.flush();
    setup_wifi();


    ArduinoOTA.setPort(otaPort);
    ArduinoOTA.setHostname(otaDeviceName);

    // No authentication by default
    ArduinoOTA.setPassword(otaPwd);

    ota.init();

    client.setServer(mqtt_server, 1883);
  //  client.setServer(mqtt_server, 9001);
    client.setCallback(callback);



  // Call io.begin(<address>) to initialize the SX1509. If it
  // successfully communicates, it'll return 1.
  if (!io_A.begin(SX1509_ADDRESS_A))
  {
    while (1) ; // If we fail to communicate, loop forever.
  }

  // Use the internal 2MHz oscillator.
  // Set LED clock to 500kHz (2MHz / (2^(3-1)):
  io_A.clock(INTERNAL_CLOCK_2MHZ, 3);

  // To breathe an LED, make sure you set it as an
  // ANALOG_OUTPUT, so we can PWM the pin:
  io_A.pinMode(SX1509_LED_PIN, ANALOG_OUTPUT);





  // Breathe an LED: 1000ms LOW, 500ms HIGH,
  // 500ms to rise from low to high
  // 250ms to fall from high to low
//   io.breathe(SX1509_LED_PIN, 1000, 500, 500, 250);
//  io_A.breathe(SX1509_LED_PIN, 0, 0, 1000, 1000);
  io_A.blink(5, 200, 200);
//  io_A.breathe(6, 1000, 250, 250, 250);

  // The timing parameters are in milliseconds, but they
  // aren't 100% exact. The library will estimate to try to
  // get them as close as possible. Play with the clock
  // divider to maybe get more accurate timing.
}

void setup_wifi() {

  WiFi.disconnect(true);
  delay(3000);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
//    Serial.printf("connecting %s:%s \r\n", ssid, password);
    Serial.printf("connecting %s \r\n", ssid);
    delay(1000);
    // Serial.print(WiFi.status());
    // Serial.print(";");
    // Serial.println(WL_CONNECTED);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



int MapLedIdToPin(int ledId) {

}


void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strcmp((const char *) topic, "plabs/blinky-1/led/set") == 0) {
    // We have received a set LED command. Process it.

    // Get MQTT message (add 1 for final 0), convert into string
    char msg[length + 1];
    memcpy(msg, payload, length);
    msg[length] = 0;

    // Read each command pair
    char* command = strtok(msg, "|");
    while (command != 0)
    {
        // Split the command in two values
        char* separator = strchr(command, ':');
        if (separator != 0)
        {
            // Actually split the string in 3: replace ':' with 0
            *separator = 0;
            char ledBank[5];   // Reserve space for a few characters. Only one should be needed (format is <bank number>, e.g. B)
            strcpy(ledBank, command);
            ++separator;

            char* separator2 = strchr(separator, ':');
            if (separator2 != 0)
            {
              *separator2 = 0;
              int ledId = atoi(separator);

              ++separator2;
              int ledMode = atoi(separator2);

              Serial.printf("ledBank=%s  ", ledBank);
              Serial.printf("ledId=%d  ", ledId);
              Serial.printf("ledMode=%d \r\n", ledMode);

              SX1509* ioActive;

              if (ledBank[0] = 'a') {
                // Bank A
                ioActive = &io_A;
              } else if (ledBank[0] = 'b') {
                // Bank B
                ioActive = &io_B;
              }

              switch (ledMode) {
                case LED_BREATH_SLOW:
                  break;
                case LED_BREATH_MEDIUM:
                  break;
                case LED_BREATH_FAST:
                  break;
                case LED_BLINK_SLOW:
                  break;
                case LED_BLINK_MEDIUM:
                  break;
                case LED_BLINK_FAST:
                  break;

              }
              ioActive->blink(MapLedIdToPin(ledId), 200, 200);

            }

            // Do something with ledId and ledMode
        }
        // Find the next command in input string
        command = strtok(0, "|");
    }

  }

}

void reconnect() {
  // Loop until we're reconnected
//  digitalWrite(BUILTIN_LED, HIGH);
  while (!client.connected()) {
//    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (client.connect("Blinky-1")) {
//      Serial.println("connected");
      // Once connected, publish an announcement.
      // Use a topic of .../ghostlamp/n/... where n is the number of the current lamp.
      // There will probably never be more than one Pacma Ghost lamp in any given network, but still...
      client.publish("plabs/blinky-1/status", "connected");

      // ... and subscribe to relevant topics
      client.subscribe("plabs/blinky-1/led/set");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void loop()
{
  ota.loop();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
