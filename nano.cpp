/*
 * Arduino Nano Code for Smart Medicine Reminder System
 * Health Monitoring Features
 *
 * This code handles:
 * - Heart rate and SpO2 measurement (MAX30100)
 * - Body temperature measurement (LM35)
 * - Room temperature and humidity (DHT11)
 * - Smoke detection (MQ2)
 * - LCD display for all measurements
 */

 #include <Wire.h>
 #include <LiquidCrystal_I2C.h>
 #include <DHT.h>
 #include <MAX30100_PulseOximeter.h>

 // Pin Definitions
 #define DHT_PIN           2      // Digital pin connected to DHT11
 #define MQ2_PIN           A1     // Analog pin connected to MQ2 smoke sensor
 #define LM35_PIN          A0     // Analog pin connected to LM35 temperature sensor
 #define MODE_SWITCH_PIN   7      // Digital pin for mode selection
 #define ISD1820_PLAY_E    9      // Play/E pin for ISD1820 module
 #define ISD1820_REC       10     // REC pin for ISD1820 module
 #define ISD1820_PLAY_L    11     // Play/L pin for ISD1820 module
 #define ISD1820_P_E      4     // P-E pin for ISD1820 module D4

 // Constants
 #define DHTTYPE           DHT11  // DHT sensor type
 #define MQ2_THRESHOLD     400    // Threshold for smoke detection
 #define REPORTING_PERIOD_MS 1000 // How often to update sensor readings

 // Global Objects
 LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
 DHT dht(DHT_PIN, DHTTYPE);
 PulseOximeter pox;

 // Global Variables
 bool defaultMode = true;         // Default mode vs Medical checkup mode
 float roomTemperature = 0;
 float roomHumidity = 0;
 float bodyTemperature = 0;
 int heartRate = 0;
 int oxygenLevel = 0;
 bool smokeDetected = false;
 unsigned long lastReportTime = 0;

 // Function called when a heartbeat is detected
 void onBeatDetected() {
   Serial.println("Beat!");
 }

 void setup() {
   Serial.begin(9600);
   Wire.begin();

   // Initialize LCD
   lcd.init();
   lcd.backlight();

   // Welcome message
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Stay Healthy.");
   lcd.setCursor(0, 1);
   lcd.print("Get Well Soon..");
   delay(2000);

   // Initialize sensors
   dht.begin();

   // Initialize pulse oximeter
   if (!pox.begin()) {
     Serial.println("Failed to initialize pulse oximeter!");
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Pulse Ox Error!");
     lcd.setCursor(0, 1);
     lcd.print("Check wiring");
   } else {
     pox.setOnBeatDetectedCallback(onBeatDetected);
   }

   // Set mode switch pin
   pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);

   // Initialize ISD1820 voice module pins
   pinMode(ISD1820_PLAY_E, OUTPUT);
   pinMode(ISD1820_REC, OUTPUT);
   pinMode(ISD1820_PLAY_L, OUTPUT);
   pinMode(ISD1820_P_E, OUTPUT);

   // Set initial states
   digitalWrite(ISD1820_PLAY_E, LOW);
   digitalWrite(ISD1820_REC, LOW);
   digitalWrite(ISD1820_PLAY_L, LOW);
   digitalWrite(ISD1820_P_E, LOW);

   Serial.println("Arduino Nano Health Monitor Initialized");
 }

 void loop() {
   // Update pulse oximeter
   pox.update();

   // Check mode switch
   defaultMode = digitalRead(MODE_SWITCH_PIN);

   // Read sensors based on mode
   if (millis() - lastReportTime > REPORTING_PERIOD_MS) {
     if (defaultMode) {
       readEnvironmentalSensors();
       displayEnvironmentalData();
     } else {
       readHealthSensors();
       displayHealthData();
     }
     lastReportTime = millis();
   }

   delay(100);   // Small delay for stability

   // Check for ISD1820 P_E signal (coming from ESP32)
   if (digitalRead(ISD1820_P_E) == HIGH) {
     playVoiceReminder();
   }
 }

 void readEnvironmentalSensors() {
   // Read DHT11 sensor (room temperature and humidity)
   roomHumidity = dht.readHumidity();
   roomTemperature = dht.readTemperature();

   // Check if reads are valid
   if (isnan(roomHumidity) || isnan(roomTemperature)) {
     Serial.println("Failed to read from DHT sensor!");
     roomHumidity = 0;
     roomTemperature = 0;
   }

   // Read MQ2 sensor (smoke detection)
   int mq2Value = analogRead(MQ2_PIN);
   smokeDetected = (mq2Value > MQ2_THRESHOLD);

   // Print values to serial for debugging
   Serial.print("Room Temp: ");
   Serial.print(roomTemperature);
   Serial.print(" °C, Humidity: ");
   Serial.print(roomHumidity);
   Serial.print("%, Smoke: ");
   Serial.println(smokeDetected ? "YES" : "NO");
 }

 void readHealthSensors() {
   // Update heart rate and SpO2 from pulse oximeter
   heartRate = pox.getHeartRate();
   oxygenLevel = pox.getSpO2();

   // Read LM35 for body temperature
   int sensorValue = analogRead(LM35_PIN);
   float voltage = sensorValue * (5.0 / 1023.0);
   bodyTemperature = voltage * 100; // LM35 gives 10mV per degree Celsius

   // Print values to serial for debugging
   Serial.print("Heart Rate: ");
   Serial.print(heartRate);
   Serial.print(" BPM, SpO2: ");
   Serial.print(oxygenLevel);
   Serial.print("%, Body Temp: ");
   Serial.println(bodyTemperature);
 }

 void displayEnvironmentalData() {
   // First display screen - check for smoke
   lcd.clear();
   if (smokeDetected) {
     lcd.setCursor(0, 0);
     lcd.print("SMOKE");
     lcd.setCursor(0, 1);
     lcd.print("DETECTED");
   } else {
     lcd.setCursor(0, 0);
     lcd.print("ENVIRONMENT");
     lcd.setCursor(0, 1);
     lcd.print("CLEAR");
   }

   delay(2000);

   // Second display screen - temperature and humidity
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Temp: ");
   lcd.print(roomTemperature, 1);
   lcd.print((char)223); // Degree symbol
   lcd.print("C");

   lcd.setCursor(0, 1);
   lcd.print("Hum: ");
   lcd.print(roomHumidity, 0);
   lcd.print("%");
 }

 void displayHealthData() {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("BPM: ");
   lcd.print(heartRate);
   lcd.print(" ");

   // Convert Celsius to Fahrenheit for display
   float tempF = (bodyTemperature * 9.0 / 5.0) + 32.0;
   lcd.print(tempF, 1);
   lcd.print((char)223); // Degree symbol
   lcd.print("F");

   lcd.setCursor(0, 1);
   lcd.print("SpO2: ");
   lcd.print(oxygenLevel);
   lcd.print("%");
 }

 void playVoiceReminder() {
   // Trigger edge-triggered playback (PLAY_E pin)
   digitalWrite(ISD1820_PLAY_E, HIGH);
   delay(100);
   digitalWrite(ISD1820_PLAY_E, LOW);

   Serial.println("Voice reminder played");

   // Wait for the message to finish (approximately 10 seconds max)
   delay(10000);
 }

 // Function to record a new voice message (can be called from setup or based on some trigger)
 void recordVoiceMessage() {
   Serial.println("Recording voice message...");

   // Start recording
   digitalWrite(ISD1820_REC, HIGH);
   delay(100);

   // Allow recording for maximum time (about 10 seconds)
   delay(10000);

   // Stop recording
   digitalWrite(ISD1820_REC, LOW);

   Serial.println("Recording complete");
 }

 // Function to play a recorded message in loop mode
 void playVoiceLoop() {
   // Trigger level-activated playback (PLAY_L pin)
   digitalWrite(ISD1820_PLAY_L, HIGH);
   delay(100);
   digitalWrite(ISD1820_PLAY_L, LOW);

   Serial.println("Voice reminder playing in loop mode");
 }