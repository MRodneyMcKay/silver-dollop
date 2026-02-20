#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <DallasTemperature.h>

class Sensor {
public:
    using Address = uint8_t[8];

    Sensor(const char* label, const uint8_t address[8]);

    // Measure temperature and update current, max, avg
    float measure(DallasTemperature& bus);

    // Print the sensor to a Print stream (Serial, etc.)
    void printTo(Print& out) const;

    // Static member function to print all sensors in an array
    static void printAll(Sensor* sensors, size_t count, Print& out) {
        for (size_t i = 0; i < count; ++i) {
            sensors[i].printTo(out);
            out.println();
        }
    }

private:
    const char* _label;
    uint8_t _address[8];

    float _current {0.0f};
    float _max { -1000.0f };
    float _avg {0.0f};
    uint32_t _samples {0};
};

// Optional operator overload for single sensor
Print& operator<<(Print& out, const Sensor& sensor);

#endif