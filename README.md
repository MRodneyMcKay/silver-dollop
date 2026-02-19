# silver-dollop

silver-dollop is a DIY temperature monitoring system built around an ESP32 and multiple Dallas 1-Wire temperature sensors (e.g. DS18B20).

The ESP32 runs a WiFi web server that:

- Detects and identifies sensors by their unique hardware addresses
- Displays live temperature readings
- Calculates per-sensor average and maximum temperature
- Shows a browser-based graph of measurements
- Logs raw data to CSV
- Provides CSV download via web interface

All sensors share a single 1-Wire data pin. The system is fully standalone and intended for hobby and DIY electronics projects.