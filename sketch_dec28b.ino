#include <Wire.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

// ====================== Configuration ======================
const char* ssid = "Santhosh-4G";
const char* password = "santhosh27";

const char* mqttServer = "192.168.29.65";
const int mqttPort = 1883;

const char* appKey = "902dc26f-3304-4055-93d7-0df6855f352b";
const char* appSecret = "c88fd465-9130-4c37-a9c3-953cdd3e950c-de4761fc-29ba-45f8-9b4f-5eefb97cce07";
const char* deviceID = "676f9eda716c0bcb08a68149";

#define TRIG_PIN 13
#define ECHO_PIN 12
#define MOISTURE_SENSOR_PIN 34

// ====================== Variables ======================
WiFiClient espClient;
PubSubClient mqttClient(espClient);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/15, /* data=*/14);

bool triggerCommands = false;

// ====================== Function Prototypes ======================
void setupWiFi();
void setupOLED();
void setupSinricPro();

void connectToMQTT();
void disconnectFromMQTT();
void sendMqttCommand(String command);
void displayMessage(const char* message);
void displayCountdown(int seconds);

bool onPowerState(const String &deviceId, bool &state);
void handleMqttResponse(char* topic, byte* payload, unsigned int length);

// ====================== Setup ======================
void setup() {
  Serial.begin(115200);
  setupOLED();
  setupWiFi();
  setupSinricPro();

  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(handleMqttResponse);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// ====================== Main Loop ======================
void loop() {
  SinricPro.handle();

  if (mqttClient.connected()) {
    mqttClient.loop();
  }
}

// ====================== Setup Modules ======================

void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    displayMessage("WiFi: Connecting...");
    Serial.print(".");
  }
  displayMessage("WiFi Connected!");
  Serial.println("WiFi Connected!");
}

void setupOLED() {
  u8g2.begin();
  displayMessage("Initializing...");
}

void setupSinricPro() {
  SinricProSwitch &mySwitch = SinricPro[deviceID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.begin(appKey, appSecret);
}

// ====================== Command Handlers ======================

void connectToMQTT() {
  while (!mqttClient.connected()) {
    displayMessage("Connecting to MQTT...");
    if (mqttClient.connect("ESP32S3Client")) {
      Serial.println("MQTT Connected!");
      mqttClient.subscribe("iot_coach/responses");
    } else {
      Serial.println("MQTT Connect Failed, retrying...");
      delay(2000);
    }
  }
}

void disconnectFromMQTT() {
  if (mqttClient.connected()) {
    mqttClient.disconnect();
    Serial.println("MQTT Disconnected!");
  }
}

void sendMqttCommand(String command) {
  if (mqttClient.publish("iot_coach/commands", command.c_str())) {
    Serial.printf("Command sent to MQTT: %s\n", command.c_str());
    displayMessage(("Command Sent: " + command).c_str());
  } else {
    Serial.println("MQTT Publish Failed");
  }
}

void handleMqttResponse(char* topic, byte* payload, unsigned int length) {
  String response = "";
  for (int i = 0; i < length; i++) {
    response += (char)payload[i];
  }
  Serial.printf("Response from IoT Coach: %s\n", response.c_str());
  displayMessage(("Response: " + response).c_str());
}

// ====================== OLED Display ======================
void displayMessage(const char* message) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 20, message);
  u8g2.sendBuffer();
}

void displayCountdown(int seconds) {
  for (int i = seconds; i > 0; i--) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(0, 20);
    u8g2.print("Next Command in:");
    u8g2.setCursor(0, 40);
    u8g2.print(i);
    u8g2.print(" seconds");
    u8g2.sendBuffer();
    delay(1000);
  }
}

// ====================== Sinric Pro Callback ======================
bool onPowerState(const String &deviceId, bool &state) {
  if (deviceId == deviceID) {
    if (state) {
      Serial.println("Alexa Command Received: Turn on S3");
      connectToMQTT();

      // Sending commands to IoT Coach with a 5-second interval
      sendMqttCommand("Enable LED");
      displayCountdown(5);

      sendMqttCommand("Enable Buzzer");
      displayCountdown(5);

      sendMqttCommand("Measure Moisture");
      displayCountdown(5);

      sendMqttCommand("Measure Distance");
      displayCountdown(5);

      sendMqttCommand("Measure DHT Sensor");
      displayCountdown(5);

      sendMqttCommand("Measure Pressure");
      displayCountdown(5);

      disconnectFromMQTT();
    } else {
      Serial.println("Alexa Command Received: Turn off S3");
    }
    return true;
  }
  return false;
}
