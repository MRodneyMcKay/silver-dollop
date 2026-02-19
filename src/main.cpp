#include <Arduino.h>
#include <OneWire.h>

// Pin where the DS18B20 data line is connected
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);

void setup() {
    Serial.begin(9600);
    while (!Serial); // wait for serial monitor
    Serial.println("1-Wire Bus Scan Starting...");
}

void loop() {
    byte address[8];
    int deviceCount = 0;

    Serial.println("Scanning 1-Wire bus...");

    oneWire.reset_search(); // start new search
    while (oneWire.search(address)) {
        deviceCount++;
        Serial.print("Device ");
        Serial.print(deviceCount);
        Serial.print(": ");
        for (int i = 0; i < 8; i++) {
            if (address[i] < 16) Serial.print("0"); // leading zero
            Serial.print(address[i], HEX);
        }

        // Check CRC
        if (OneWire::crc8(address, 7) != address[7]) {
            Serial.println("  CRC INVALID");
        } else {
            Serial.println("  CRC OK");
        }
    }

    if (deviceCount == 0) {
        Serial.println("No devices found.");
    }

    Serial.println("Scan complete.");
    delay(5000); // scan every 5 seconds
}