#define BLYNK_TEMPLATE_ID "TMPL36KbMVmX6"
#define BLYNK_TEMPLATE_NAME "S3"
#define BLYNK_AUTH_TOKEN "QZcYZkq18e7P806rYmcF61381GmMu-bl"

#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>

// ================== Configuration ==================
// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// GPIO Definitions
#define LED_PIN 2
#define BUZZER_PIN 27
#define TRIG_PIN 13
#define ECHO_PIN 12
#define MOISTURE_SENSOR_PIN 34
#define DHT_PIN 25 // GPIO pin for DHT22 sensor

// Sensor Definitions
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);
Adafruit_BMP280 bmp; // Pressure sensor

// WiFi Credentials
const char* ssid = "Santhosh-4G";
const char* password = "santhosh27";

// MQTT Settings
const char* mqttServer = "192.168.29.65";
const int mqttPort = 1883;
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Blynk Settings
char blynkAuth[] = BLYNK_AUTH_TOKEN;

// ================== Function Prototypes ==================
void initializeOLED();
void initializeSensors();
void connectToWiFi();
void connectToMQTT();
void connectToBlynk();
void processCommand(char* topic, byte* payload, unsigned int length);
void sendResponse(const char* response);
void displayMessage(const char* message);
float measureDistance();
int readMoistureLevel();
void readDHTSensor();
void readPressureSensor();
void sendSensorDataToBlynk();

// ================== Global Variables ==================
float lastDistance = -1.0;
int lastMoisture = -1;
float lastTemperature = -1.0;
float lastHumidity = -1.0;
float lastPressure = -1.0;

// ================== Setup Function ==================
void setup() {
  Serial.begin(115200);

  // Initialize Components
  initializeOLED();
  initializeSensors();
  connectToWiFi();
  connectToMQTT();
  connectToBlynk();

  // Initialize GPIO
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MOISTURE_SENSOR_PIN, INPUT);
}

// ================== Loop Function ==================
void loop() {
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();
  Blynk.run();
}

// ================== OLED Initialization ==================
void initializeOLED() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Correct initialization
    Serial.println("SSD1306 initialization failed!");
    while (1); // Stay here if OLED initialization fails
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("OLED Initialized!");
  display.display();
  delay(2000); // Show the message for 2 seconds
}

// ================== Sensor Initialization ==================
void initializeSensors() {
  dht.begin();
  Serial.println("DHT22 initialized.");
  if (!bmp.begin(0x76)) { // 0x76 is the default I2C address for BMP280
    Serial.println("BMP280 initialization failed!");
    displayMessage("BMP280 Error!");
  } else {
    Serial.println("BMP280 initialized.");
  }
}

// ================== WiFi Connection ==================
void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    displayMessage("WiFi: Connecting...");
    delay(500);
  }
  displayMessage("WiFi Connected!");
  Serial.println("WiFi Connected!");
}

// ================== MQTT Connection ==================
void connectToMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(processCommand);

  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("MQTT Connected!");
      mqttClient.subscribe("iot_coach/commands");
    } else {
      delay(5000); // Retry connection every 5 seconds
    }
  }
}

// ================== Blynk Connection ==================
void connectToBlynk() {
  Blynk.config(blynkAuth);
  while (!Blynk.connected()) {
    Blynk.run();
    delay(500);
  }
  Serial.println("Blynk Connected!");
}

// ================== MQTT Command Processing ==================
void processCommand(char* topic, byte* payload, unsigned int length) {
  String command = "";
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }
  Serial.print("Command received: ");
  Serial.println(command);

  if (command == "Enable LED") {
    digitalWrite(LED_PIN, HIGH);
    displayMessage("LED ON");
    delay(5000);
    digitalWrite(LED_PIN, LOW);
    sendResponse("LED Toggled");
  } else if (command == "Enable Buzzer") {
    digitalWrite(BUZZER_PIN, HIGH);
    displayMessage("Buzzer ON");
    delay(5000);
    digitalWrite(BUZZER_PIN, LOW);
    sendResponse("Buzzer Toggled");
  } else if (command == "Measure Distance") {
    lastDistance = measureDistance();
    sendSensorDataToBlynk();
  } else if (command == "Measure Moisture") {
    lastMoisture = readMoistureLevel();
    sendSensorDataToBlynk();
  } else if (command == "Measure DHT Sensor") {
    readDHTSensor();
    sendSensorDataToBlynk();
  } else if (command == "Measure Pressure") {
    readPressureSensor();
    sendSensorDataToBlynk();
  } else {
    sendResponse("Unknown Command");
    displayMessage("Unknown Command");
  }
}

// ================== Sensor Readings ==================
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return (duration == 0) ? -1 : (duration * 0.034) / 2;
}

int readMoistureLevel() {
  int moistureValue = analogRead(MOISTURE_SENSOR_PIN);
  return map(moistureValue, 0, 4095, 0, 100);
}

void readDHTSensor() {
  lastTemperature = dht.readTemperature();
  lastHumidity = dht.readHumidity();

  if (isnan(lastTemperature) || isnan(lastHumidity)) {
    sendResponse("Failed to read from DHT22 sensor!");
    displayMessage("DHT22 Error");
    return;
  }

  char response[64];
  snprintf(response, sizeof(response), "Temp: %.2fC, Humidity: %.2f%%", lastTemperature, lastHumidity);
  sendResponse(response);
  displayMessage(response);
}

void readPressureSensor() {
  lastPressure = bmp.readPressure() / 100.0F;
  char response[32];
  snprintf(response, sizeof(response), "Pressure: %.2f hPa", lastPressure);
  sendResponse(response);
  displayMessage(response);
}

// ================== Data to Blynk ==================
void sendSensorDataToBlynk() {
  Blynk.virtualWrite(V0, lastDistance);
  Blynk.virtualWrite(V1, lastMoisture);
  Blynk.virtualWrite(V2, lastTemperature);
  Blynk.virtualWrite(V3, lastHumidity);
  Blynk.virtualWrite(V4, lastPressure);
}

// ================== Helper Functions ==================
void displayMessage(const char* message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print(message);
  display.display();
}

void sendResponse(const char* response) {
  if (mqttClient.connected()) {
    mqttClient.publish("esp32s3/responses", response);
    Serial.println("Response sent: " + String(response));
  }
}
