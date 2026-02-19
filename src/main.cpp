#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "sensors.h"

// Pin where DS18B20 data line is connected
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorsDevice(&oneWire);

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Single sensor temperature test");

    // Initialize the DallasTemperature library
    sensorsDevice.begin();
}

void loop() {
    sensorsDevice.requestTemperatures(); // ask sensor to measure

    for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
        float tempC = sensorsDevice.getTempC(sensors[i].address);
        if (tempC == DEVICE_DISCONNECTED_C) {
            Serial.print(sensors[i].label);
            Serial.println(": Sensor disconnected!");
        } else {
            Serial.print(sensors[i].label);
            Serial.print(": ");
            Serial.print(tempC);
            Serial.println(" °C");
        }
    }

    Serial.println("----------------------");
    delay(2000); // read every 2 seconds
}