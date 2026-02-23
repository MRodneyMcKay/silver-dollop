#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <DallasTemperature.h>
#include <string.h>  // AVR-safe replacement for cstring

class Sensor {
public:
    using Address = uint8_t[8];

    // Constructor: label + sensor address
    Sensor(const char* label, const uint8_t address[8]);

    // Measure temperature and update current, max, EMA average
    float measure(DallasTemperature& bus, float alpha = 0.05f);

    // Print the sensor to a Print stream
    void printTo(Print& out) const;

    // Static member function to print all sensors in an array
    template <size_t N> 
    static void printAll(Sensor (&sensors)[N], Print& out) { 
        for (auto& s : sensors) { 
            s.printTo(out);
            out.println(); 
        } 
    }

    struct Measurement {
        const char* label;
        float value;
    };

    template <size_t N>
    struct MeasurementArray {
        Measurement data[N];

        constexpr size_t size() const { return N; }

        Measurement& operator[](size_t i) { return data[i]; }
        const Measurement& operator[](size_t i) const { return data[i]; }

        Measurement* begin() { return data; }
        Measurement* end() { return data + N; }
        const Measurement* begin() const { return data; }
        const Measurement* end() const { return data + N; }
    };

    template <size_t N>
    static MeasurementArray<N>
    measureAll(Sensor (&sensors)[N], DallasTemperature& bus)
    {
        MeasurementArray<N> results;

        bus.requestTemperatures();

        size_t index = 0;
        for (auto& s : sensors) {
            results.data[index++] = { s._label, s.measure(bus) };
        }

        return results;
    }

private:
    const char* _label;
    uint8_t _address[8];

    void initializeState();
    float _current;
    float _max;
    float _ema;
    uint32_t _samples;

    // Delete copy/move to avoid accidental copies
    Sensor(const Sensor&) = delete;
    Sensor& operator=(const Sensor&) = delete;
};

#endif