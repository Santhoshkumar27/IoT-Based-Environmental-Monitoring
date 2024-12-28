# IoT-Based-Environmental-Monitoring
This project presents the development of an IoT-based environmental monitoring and control system. The system integrates sensors for temperature, humidity, soil moisture, distance, and atmospheric pressure with an ESP32 microcontroller. It provides real-time data visualization on an OLED display and enables remote monitoring 

IoT-Based Environmental Monitoring and Control with Alexa Integration Using ESP32 and OLED Display
– Santhosh Kumar E | Intern | AIoT Batch 8 | Sabudh
Abstract
This project presents the development of an IoT-based environmental monitoring and control system. The system integrates sensors for temperature, humidity, soil moisture, distance, and atmospheric pressure with an ESP32 microcontroller. It provides real-time data visualization on an OLED display and enables remote monitoring and control through MQTT, Blynk, and Alexa (via SinricPro). The project is designed for smart agriculture, environmental monitoring, and home automation applications, emphasizing modularity and ease of use.
Introduction
The Internet of Things (IoT) has transformed the way devices interact by enabling real-time data communication and control. This project utilizes the ESP32 microcontroller to develop a versatile system that monitors environmental parameters, displays them locally on an OLED screen, and provides remote control capabilities using Alexa voice commands, MQTT, and the Blynk platform.
Objectives
Multi-Sensor Integration: Monitor environmental parameters like temperature, humidity, pressure, soil moisture, and distance.
OLED Display: Provide real-time visualization of sensor data.
Alexa Integration: Allow voice-based control using Amazon Alexa via SinricPro.
Cloud Connectivity: Facilitate remote monitoring and control via MQTT and Blynk.
Actuator Control: Enable control of LED and buzzer for alerts and feedback.
Error Handling: Ensure robust operation by displaying errors on the OLED.
System Components
Microcontroller
ESP32:
Features: Dual-core processor, built-in WiFi and Bluetooth, multiple GPIO pins.
Purpose: Acts as the central hub for sensor data acquisition, processing, and communication with cloud platforms.
Sensors
DHT22:
Measures temperature and humidity.
Connected to GPIO 25.
Pull-up resistor (10kΩ) used between VCC and Data pin for stable operation.
BMP280:
Measures atmospheric pressure.
Connected via I2C to GPIO 21 (SDA) and GPIO 22 (SCL).
I2C address: 0x76.
Soil Moisture Sensor:
Measures soil moisture level.
Analog output connected to GPIO 34.
Ultrasonic Distance Sensor (HC-SR04):
Measures distance to obstacles.
TRIG pin connected to GPIO 13 and ECHO pin to GPIO 12.
Display
Adafruit SSD1306 OLED Display:
Resolution: 128x64 pixels.
Interface: I2C.
Connected to GPIO 21 (SDA) and GPIO 22 (SCL).
Actuators
LED:
Provides visual feedback.
Connected to GPIO 2.
Buzzer:
Alerts users of specific conditions.
Connected to GPIO 27.
Cloud Integration
MQTT:
Broker: Mosquitto server at 192.168.29.65.
Handles message communication between devices and cloud.
Blynk:
Provides a user-friendly IoT dashboard for remote monitoring.
SinricPro:
Integrates Alexa for voice control.
Device ID: Linked to SinricPro for secure communication.
System Architecture
Local System:
ESP32 acquires data from connected sensors.
Data is displayed on the OLED in real-time.
Cloud Communication:
Sensor data is sent to MQTT and Blynk platforms.
Commands from Alexa are processed via SinricPro for controlling actuators.
User Interaction:
Local: OLED for real-time data.
Remote: Blynk dashboard, MQTT, and Alexa voice commands.
Implementation
Sensor Integration
DHT22: Used for temperature and humidity measurements. Configured using the DHT library.
BMP280: Configured via the Adafruit BMP280 library for accurate pressure readings.
Soil Moisture Sensor: Provides analog output representing soil moisture percentage.
Ultrasonic Sensor: Uses pulse width to calculate distance in cm.
OLED Initialization
Configured using the Adafruit SSD1306 library.
Displays connection status, sensor readings, and error messages.
Alexa Integration
Configured using SinricPro for Alexa voice control.
Functions implemented for LED and buzzer toggling.
Cloud Communication
MQTT: Publishes sensor data to the cloud and subscribes to commands.
Blynk: Updates virtual pins with real-time data for remote monitoring.
Results
Real-Time Monitoring:
OLED displayed temperature, humidity, pressure, distance, and soil moisture in real-time.
Data sent to Blynk dashboard for remote monitoring.
Voice Control:
Alexa successfully toggled LED and buzzer via SinricPro.
Error Handling:
Displayed errors for sensor initialization failures on the OLED.
Applications
Smart Agriculture:
Optimize irrigation using soil moisture data.
Monitor environmental conditions for crop health.
Environmental Monitoring:
Measure atmospheric pressure for weather forecasting.
Track temperature and humidity for climate studies.
Home Automation:
Integrate with Alexa for smart home devices.
Conclusion
The IoT-based monitoring system successfully integrates multiple sensors with ESP32 for real-time data visualization and remote control. The addition of Alexa voice control enhances usability. The system's modular design allows easy scalability and adaptability for various applications.
Future Enhancements
Add additional sensors like CO2 or air quality.
Integrate predictive analytics using machine learning.
Develop a mobile app for improved user interaction.
