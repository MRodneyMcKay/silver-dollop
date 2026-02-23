#include <Arduino.h>
#include <OneWire.h>
#include <unity.h>

// Define your 1-Wire pin here
#define ONE_WIRE_BUS 4 

OneWire ds(ONE_WIRE_BUS);

void test_device_crc(void) {
    byte addr[8];
    bool deviceFound = false;

    Serial.println("\n--- Starting 1-Wire Scan ---");
    ds.reset_search();

    while (ds.search(addr)) {
        deviceFound = true;
        
        // Print the ID in Hex
        Serial.print("Found ID: ");
        for (int i = 0; i < 8; i++) {
            if (addr[i] < 16) Serial.print("0");
            Serial.print(addr[i], HEX);
            if (i < 7) Serial.print(":");
        }

        // Perform the CRC check
        // OneWire::crc8(data, length) calculates the CRC for the first 7 bytes
        // and compares it to the 8th byte (addr[7])
        uint8_t calculatedCRC = OneWire::crc8(addr, 7);
        
        if (calculatedCRC == addr[7]) {
            Serial.println(" [CRC OK]");
        } else {
            Serial.print(" [CRC ERROR! Expected: ");
            Serial.print(addr[7], HEX);
            Serial.print(" Got: ");
            Serial.print(calculatedCRC, HEX);
            Serial.println("]");
        }

        // Unity Assertion: This will fail the test if any device has a bad CRC
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(addr[7], calculatedCRC, "CRC mismatch detected on 1-Wire device!");
    }

    if (!deviceFound) {
        Serial.println("No 1-Wire devices found on pin.");
        TEST_FAIL_MESSAGE("No 1-Wire devices found. Check your wiring/pull-up resistor.");
    }
    
    ds.reset_search();
}

void setup() {
    // Wait for hardware to initialize
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_device_crc);
    UNITY_END();
}

void loop() {
    // PlatformIO tests usually finish in setup()
}
