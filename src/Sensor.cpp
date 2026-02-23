#include "Sensor.h"
#include <string.h> // for memcpy

// Constructor
Sensor::Sensor(const char* label, const uint8_t address[8])
    : _label(label)
{
    memcpy(_address, address, 8);
}

// Measure temperature and update EMA
float Sensor::measure(DallasTemperature& bus, float alpha)
{
    float temp = bus.getTempC(_address);

    if (temp == DEVICE_DISCONNECTED_C) {
        return temp;
    }

    _current = temp;

    if (_samples == 0) {
        _max = temp;
        _ema = temp; // EMA starts with first value
    } else {
        if (temp > _max) _max = temp;
        _ema = (1.0f - alpha) * _ema + alpha * temp;
    }

    ++_samples;
    return _current;
}

// Print the sensor to a Print stream
void Sensor::printTo(Print& out) const
{
    out.print(_label);
    out.print(": ");
    out.print(_current, 2);
    out.print(" °C | EMA avg: ");
    out.print(_ema, 2);
    out.print(" °C | max: ");
    out.print(_max, 2);
    out.print(" °C");
}