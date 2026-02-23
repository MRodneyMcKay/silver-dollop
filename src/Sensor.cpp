#include "Sensor.h"
#include <string.h> // for memcpy

// Constructor
Sensor::Sensor(const char* label, const uint8_t address[8])
    : _label(label)
{
    memcpy(_address, address, 8);
    initializeState();
}

void Sensor::initializeState() {
    _current = DEVICE_DISCONNECTED_C;
    _max = DEVICE_DISCONNECTED_C;
    _ema = DEVICE_DISCONNECTED_C;
    _samples = 0;
}

// Measure temperature and update EMA
float Sensor::measure(DallasTemperature& bus, float alpha)
{
    float temp = bus.getTempC(_address);

    if (temp == DEVICE_DISCONNECTED_C) {
        initializeState();
        return DEVICE_DISCONNECTED_C;
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
    if (_current == DEVICE_DISCONNECTED_C) {
        out.print("[DISCONNECTED]");
    } else {
        out.print(_current, 2);
        out.print(" °C | EMA avg: ");
        out.print(_ema, 2);
        out.print(" °C | max: ");
        out.print(_max, 2);
        out.print(" °C");
    }  
}

void Sensor::printUptime(Print& out) {
    uint32_t sec = millis() / 1000;
    int h = sec / 3600;
    int m = (sec % 3600) / 60;
    int s = sec % 60;

    if (h < 10) out.print('0');
    out.print(h);
    out.print(':');
    if (m < 10) out.print('0');
    out.print(m);
    out.print(':');
    if (s < 10) out.print('0');
    out.print(s);
}