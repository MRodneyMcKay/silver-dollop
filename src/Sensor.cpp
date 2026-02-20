#include "Sensor.h"
#include <string.h>

Sensor::Sensor(const char* label, const uint8_t address[8])
    : _label(label)
{
    memcpy(_address, address, 8);
}

float Sensor::measure(DallasTemperature& bus)
{
    float temp = bus.getTempC(_address);

    if (temp == DEVICE_DISCONNECTED_C) {
        return temp;
    }

    _current = temp;

    if (_samples == 0) {
        _max = temp;
        _avg = temp;
    } else {
        if (temp > _max) {
            _max = temp;
        }
        _avg = ((_avg * _samples) + temp) / (_samples + 1);
    }

    ++_samples;
    return _current;
}

void Sensor::printTo(Print& out) const
{
    out.print(_label);
    out.print(": ");
    out.print(_current, 2);
    out.print(" °C | avg: ");
    out.print(_avg, 2);
    out.print(" °C | max: ");
    out.print(_max, 2);
    out.print(" °C");
}

// Optional operator overload
Print& operator<<(Print& out, const Sensor& sensor)
{
    sensor.printTo(out);
    return out;
}