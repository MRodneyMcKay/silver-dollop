#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Sensor.h"

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature bus(&oneWire);

// Static sensor array
Sensor sensors[] = {
    Sensor("Sensor 01", (uint8_t[]){0x28,0xC5,0x8F,0x55,0x05,0x00,0x00,0xA2}),
    Sensor("Sensor 02", (uint8_t[]){0x28,0x45,0x39,0x56,0x05,0x00,0x00,0x13}),
    Sensor("Sensor 03", (uint8_t[]){0x28,0xE2,0x74,0x55,0x05,0x00,0x00,0x47}),
    Sensor("Sensor 04", (uint8_t[]){0x28,0xBE,0xBA,0x55,0x05,0x00,0x00,0x94})
};

constexpr size_t SENSOR_COUNT = sizeof(sensors)/sizeof(Sensor);

void setup() {
    Serial.begin(9600);
    bus.begin();
}

void loop() {
    bus.requestTemperatures();

    // Measure all sensors
    for (size_t i = 0; i < SENSOR_COUNT; ++i) {
        sensors[i].measure(bus);
    }

    // Print all sensors using static member function
    Sensor::printAll(sensors, SENSOR_COUNT, Serial);

    Serial.println("---------------------------");
    delay(2000);
}