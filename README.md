# BMP280-esp32-openHAB
Home Weather Station
A simple IoT weather station based on ESP32-WROOM-32 and BMP280.
The device measures temperature and pressure, sends data via MQTT, and integrates with OpenHAB.
It also supports Telegram alerts when temperature gets too high.

Review the code and comments. Enter the following in the appropriate spaces:
- the Wi-Fi network password and name,
- the IP address of your server (e.g., from a Raspberry Pi or laptop).

You can download the openHAB phone app, which allows you to monitor the status of the parameters on your phone.

Features:
- temperature measurement,
- pressure measurement,
- MQTT communication,
- OpenHAB integration,
- Telegram temperature alerts,
- low power operation using deep sleep.

Hardware:
- ESP32-WROOM-32,
- BMP280 sensor,
- jumpers.

Wiring:
| BMP280 | ESP32  |
| ------ | ------ |
| VCC    | 3.3V   |
| GND    | GND    |
| SDA    | GPIO22 |
| SCL    | GPIO21 |

OpenHAB Alerts
The system includes 3 simple temperature alert rules:
1. Sends a Telegram alert when temperature rises above 30°C
2. Sends a reminder every 30 minutes if temperature is still above 30°C
3. Resets the alert when temperature drops below 29.5°C
This prevents repeated notifications and adds simple hysteresis.

Power Saving
After sending data, the ESP32 enters deep sleep mode to reduce power consumption.

Technologies Used:
- Arduino IDE
- MQTT
- OpenHAB
- Telegram Bot
