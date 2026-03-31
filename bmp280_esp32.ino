#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_sleep.h>

// BMP280 I2C address
#define BMP280_ADDRESS 0x76

// I2C pins
#define I2C_SDA_PIN 22
#define I2C_SCL_PIN 21

// MQTT settings
#define MQTT_PORT 1883

// Deep sleep time (in seconds)
#define SLEEP_SECONDS 300 // 5 minutes

// WiFi credentials
const char* WIFI_SSID = "xxx";      // Enter yours 
const char* WIFI_PASSWORD = "xxx";  // Enter yours 

// MQTT broker IP address
const char* MQTT_SERVER = "xxx";    // Enter yours 

// BMP280 sensor object
Adafruit_BMP280 bmp;

// WiFi and MQTT client objects
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Function to put ESP32 into deep sleep mode
void enterDeepSleep() {
  Serial.println("Entering deep sleep...");
  Serial.flush();                     // Flush serial output
  WiFi.disconnect(true);              // Disconnect WiFi and clear connection config
  WiFi.mode(WIFI_OFF);                // Turn off WiFi to reduce power consumption
  btStop();                           // Turn off Bluetooth (if enabled)
  esp_sleep_enable_timer_wakeup((uint64_t)SLEEP_SECONDS * 1000000ULL);
  esp_deep_sleep_start();
}

void setup() {
  // Start serial communication
  Serial.begin(9600);
  delay(1200);                        // Wait for serial monitor startup

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize I2C communication
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // Check if BMP280 sensor is connected properly
  if (!bmp.begin(BMP280_ADDRESS)) {
    Serial.println("Error: BMP280 not found! Check the wiring.");
    while (1);
  } else {
    Serial.println("BMP280 sensor detected");
  }

  // Set MQTT broker
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

  // Configure BMP280 sensor settings
  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,     
    Adafruit_BMP280::SAMPLING_X16,    
    Adafruit_BMP280::FILTER_X16,      
    Adafruit_BMP280::STANDBY_MS_500   
  );
}

void loop() {
  // Connect to MQTT broker if not already connected
  if (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32_BMP280")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("MQTT connection error: ");
      Serial.println(mqttClient.state());
      delay(2000);
      enterDeepSleep();               // Go to sleep if MQTT connection fails
    }
  }

  // Read temperature and pressure values
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;

  // Print readings to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C, Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Convert values to text format for MQTT publishing
  char temperatureText[16];
  char pressureText[16];

  snprintf(temperatureText, sizeof(temperatureText), "%.1f", temperature);
  snprintf(pressureText, sizeof(pressureText), "%.2f", pressure);

  // Publish sensor readings to MQTT topics
  // Pay attention to the path
  mqttClient.publish("home/esp32/bmp280/temperature", temperatureText, true);
  mqttClient.publish("home/esp32/bmp280/pressure", pressureText, true);

  // Finish MQTT communication
  mqttClient.loop();
  delay(300);
  mqttClient.disconnect();
  delay(300);

  // Put ESP32 into deep sleep mode
  enterDeepSleep();
}