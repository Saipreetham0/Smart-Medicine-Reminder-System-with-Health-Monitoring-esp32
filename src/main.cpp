// /*
//  * Smart Medicine Reminder Kit
//  * ESP32 Code for PlatformIO
//  *
//  * Features:
//  * - User authentication with login/signup
//  * - Medicine schedule management
//  * - LED indicators for medicine chambers
//  * - Reed switch detection for medicine boxes
//  * - GSM module for phone call reminders
//  * - RTC integration for time tracking
//  */

//  #include <Arduino.h>
//  #include <WiFi.h>
//  #include <AsyncTCP.h>
//  #include <ESPAsyncWebServer.h>
//  #include <Wire.h>
//  #include <RTClib.h>
//  #include <LittleFS.h>
//  #include <ArduinoJson.h>

//  // Include project headers
//  #include "data_structures.h"
//  #include "file_operations.h"
//  #include "web_server.h"
//  #include "gsm_module.h"

//  // Pin Definitions for Medicine Chambers
//  #define LED_1 13
//  #define LED_2 12
//  #define LED_3 14
//  #define LED_4 27
//  #define LED_5 26
//  #define LED_6 25
//  #define LED_7 33
//  #define LED_8 32
//  const int LED_PINS[] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8};
//  const int NUM_CHAMBERS = 8;

//  // Reed Switch Pin (all reed switches connected to this pin)
//  #define REED_PIN 19

//  // Pin for Buzzer
//  #define BUZZER_PIN 5

//  // GSM Module pins
//  #define GSM_RX 3
//  #define GSM_TX 1

//  // WiFi Access Point Settings
//  const char* ssid = "Medicine Reminder Point";
//  const char* password = "MedReminder123";

//  // Web Server
//  AsyncWebServer server(80);

//  // RTC Module
//  RTC_DS3231 rtc;

//  // GSM Module
//  GSMModule gsm(GSM_RX, GSM_TX);

//  // Global Variables
//  MedicineSchedule schedules[20]; // Support up to 20 medicine schedules
//  int numSchedules = 0;
//  String patientPhone = "";
//  const int CALL_DELAY_MS = 300000; // 5 minutes (300,000 ms)
//  const int ALARM_DURATION_MS = 30000; // 30 seconds

//  #define MAX_USERS 10
//  User users[MAX_USERS];
//  int numUsers = 0;

//  // Function declarations
//  void checkSchedules();
//  void checkReedSwitches();

//  void setup() {
//    Serial.begin(115200);
//    Serial.println("\n\nStarting Smart Medicine Reminder Kit...");

//    // Initialize pins
//    for (int i = 0; i < NUM_CHAMBERS; i++) {
//      pinMode(LED_PINS[i], OUTPUT);
//      digitalWrite(LED_PINS[i], LOW);
//    }
//    pinMode(REED_PIN, INPUT_PULLUP);
//    pinMode(BUZZER_PIN, OUTPUT);
//    digitalWrite(BUZZER_PIN, LOW);

//    // Initialize LittleFS
//    if (!LittleFS.begin(true)) {
//      Serial.println("An error occurred while mounting LittleFS");
//      return;
//    }
//    Serial.println("LittleFS mounted successfully");

//    // Initialize RTC
//    if (!rtc.begin()) {
//      Serial.println("Couldn't find RTC");
//      while (1) delay(10);
//    }
//    Serial.println("RTC initialized");

//    // Set the RTC to the date & time this sketch was compiled
//    // Uncomment this line to set the time (run once, then comment out again)
//    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

//    // Print current time from RTC
//    DateTime now = rtc.now();
//    Serial.print("Current RTC time: ");
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(" ");
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.println(now.second(), DEC);

//    // Initialize GSM module
//    gsm.begin(9600);
//    Serial.println("GSM module initialized");

//    // Set up WiFi Access Point
//    WiFi.softAP(ssid, password);
//    delay(100);
//    IPAddress IP = WiFi.softAPIP();
//    Serial.print("AP IP address: ");
//    Serial.println(IP);

//    // Load users and schedules
//    loadUsers(users, numUsers);
//    loadSchedules(schedules, numSchedules);
//    patientPhone = loadPatientPhone();

//    // Set up web server routes
//    setupWebServer(server, schedules, numSchedules, users, numUsers, patientPhone);

//    // Start server
//    server.begin();
//    Serial.println("HTTP server started");
//    Serial.println("Smart Medicine Reminder Kit is ready!");
//  }

//  void loop() {
//    // Check medicine schedules
//    checkSchedules();

//    // Check if medicine is taken
//    checkReedSwitches();

//    delay(100);
//  }

//  // Check if medicine is taken by monitoring reed switch
//  void checkReedSwitches() {
//    // Read the reed switch state (HIGH when box is opened)
//    bool boxOpened = (digitalRead(REED_PIN) == HIGH);

//    if (boxOpened) {
//      // If any box is opened, check which LED is currently active
//      for (int i = 0; i < numSchedules; i++) {
//        if (schedules[i].alarmActive && !schedules[i].taken) {
//          int chamberIndex = schedules[i].chamberIndex;

//          if (chamberIndex >= 0 && chamberIndex < NUM_CHAMBERS) {
//            // Since we can't know which specific chamber was opened,
//            // we assume the active one was opened
//            schedules[i].taken = true;
//            schedules[i].alarmActive = false;

//            // Turn off the LED for this chamber
//            digitalWrite(LED_PINS[chamberIndex], LOW);

//            // Turn off the buzzer
//            digitalWrite(BUZZER_PIN, LOW);

//            Serial.print("Medicine taken for schedule ");
//            Serial.println(i);

//            // Save updated schedules
//            saveSchedules(schedules, numSchedules);
//          }
//        }
//      }
//    }
//  }

//  // Check schedules and activate alarms
//  void checkSchedules() {
//    DateTime now = rtc.now();
//    int currentHour = now.hour();
//    int currentMinute = now.minute();

//    for (int i = 0; i < numSchedules; i++) {
//      // If the schedule matches current time and not already taken
//      if (schedules[i].hour == currentHour &&
//          schedules[i].minute == currentMinute &&
//          !schedules[i].taken &&
//          !schedules[i].alarmActive) {

//        // Activate the alarm
//        schedules[i].alarmActive = true;
//        schedules[i].alarmStartTime = millis();

//        // Turn on the LED for the specific chamber
//        if (schedules[i].chamberIndex >= 0 && schedules[i].chamberIndex < NUM_CHAMBERS) {
//          digitalWrite(LED_PINS[schedules[i].chamberIndex], HIGH);
//        }

//        // Sound the buzzer
//        digitalWrite(BUZZER_PIN, HIGH);

//        Serial.print("Alarm activated for schedule ");
//        Serial.println(i);
//      }

//      // Check if alarm should be turned off after the duration
//      if (schedules[i].alarmActive &&
//          (millis() - schedules[i].alarmStartTime > ALARM_DURATION_MS) &&
//          !schedules[i].taken) {

//        // Turn off the buzzer
//        digitalWrite(BUZZER_PIN, LOW);

//        Serial.print("Alarm duration ended for schedule ");
//        Serial.println(i);
//      }

//      // Check if we should make a phone call
//      if (schedules[i].alarmActive &&
//          !schedules[i].taken &&
//          !schedules[i].callSent &&
//          (millis() - schedules[i].alarmStartTime > CALL_DELAY_MS)) {

//        // If patient phone is set, make the call
//        if (patientPhone.length() > 0) {
//          Serial.print("Making call to ");
//          Serial.println(patientPhone);
//          gsm.makeCall(patientPhone);
//          schedules[i].callSent = true;

//          // Save updated schedules
//          saveSchedules(schedules, numSchedules);
//        }
//      }
//    }
//  }


/*
 * Smart Medicine Reminder Kit
 * ESP32 Code for PlatformIO
 *
 * Features:
 * - Medicine scheduling and reminders
 * - LED indicators for medicine chambers
 * - Reed switch detection for medicine boxes
 * - GSM module for phone call reminders
 * - RTC for time tracking
 * - Web interface for configuration
 */

 #include <Arduino.h>
 #include <WiFi.h>
 #include <AsyncTCP.h>
 #include <ESPAsyncWebServer.h>
 #include <Wire.h>
 #include <RTClib.h>
 #include <LittleFS.h>
 #include <ArduinoJson.h>
 #include <SoftwareSerial.h>

 // Include project headers
 #include "data_structures.h"
 #include "file_operations.h"
 #include "web_server.h"
 #include "gsm_module.h"

 // Pin Definitions for Medicine Chambers
 #define LED_1 13
 #define LED_2 12
 #define LED_3 14
 #define LED_4 27
 #define LED_5 26
 #define LED_6 25
 #define LED_7 33
 #define LED_8 32
 const int LED_PINS[] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8};
 const int NUM_CHAMBERS = 8;

 // Reed Switch Pin (all reed switches connected to this pin)
 #define REED_PIN 19

 // Pin for Buzzer
 #define BUZZER_PIN 5

 // GSM Module pins
 #define GSM_RX 16
 #define GSM_TX 17

 // WiFi Access Point Settings
 const char* ssid = "Medicine Reminder";
 const char* password = "medkit123";

 // Web Server
 AsyncWebServer server(80);

 // RTC Module
 RTC_DS3231 rtc;

 // GSM Module
 GSMModule gsm(GSM_RX, GSM_TX);

 // Global Variables
 MedicineSchedule schedules[20]; // Support up to 20 medicine schedules
 int numSchedules = 0;
 String patientPhone = "";
 const int CALL_DELAY_MS = 300000; // 5 minutes (300,000 ms)
 const int ALARM_DURATION_MS = 30000; // 30 seconds

 #define MAX_USERS 10
 User users[MAX_USERS];
 int numUsers = 0;



 // Add this standalone function to your main.cpp file

bool emergency_save_schedule(int chamber, int hour, int minute) {
    // Try to format the filesystem if it's causing problems
    // Uncomment this line only if you want to reset everything
    // LittleFS.format();

    // Create a very simple JSON structure
    String json = "{\"chamber\":" + String(chamber) +
                  ",\"hour\":" + String(hour) +
                  ",\"minute\":" + String(minute) + "}";

    // Try to save as a simple text file
    File file = LittleFS.open("/simple_schedule.txt", "w");
    if (!file) {
      Serial.println("ERROR: Could not open file for writing");
      return false;
    }

    // Write the data
    if (file.print(json) == 0) {
      Serial.println("ERROR: Failed to write to file");
      file.close();
      return false;
    }

    file.close();
    Serial.println("SUCCESS: Schedule saved to simple file");

    // Now try to read it back to verify
    file = LittleFS.open("/simple_schedule.txt", "r");
    if (!file) {
      Serial.println("ERROR: Could not open file for reading");
      return false;
    }

    String content = file.readString();
    file.close();

    Serial.print("Read back: ");
    Serial.println(content);

    return true;
  }

 // Function declarations
 void checkSchedules();
 void checkReedSwitches();

//  void setup() {
//    Serial.begin(115200);
//    delay(1000);
//    Serial.println("\n\nStarting Smart Medicine Reminder Kit...");

//    // Initialize pins
//    for (int i = 0; i < NUM_CHAMBERS; i++) {
//      pinMode(LED_PINS[i], OUTPUT);
//      digitalWrite(LED_PINS[i], LOW);
//    }
//    pinMode(REED_PIN, INPUT_PULLUP);
//    pinMode(BUZZER_PIN, OUTPUT);
//    digitalWrite(BUZZER_PIN, LOW);

//    // Initialize LittleFS
//    if (!LittleFS.begin(false)) {
//      Serial.println("An error occurred while mounting LittleFS. Trying to format...");
//      if (!LittleFS.begin(true)) {
//        Serial.println("LittleFS mount failed even after formatting");
//      } else {
//        Serial.println("LittleFS formatted and mounted successfully");
//      }
//    } else {
//      Serial.println("LittleFS mounted successfully");
//    }

//    // Initialize I2C and RTC
//    Wire.begin();
//    delay(100);

//    // Initialize RTC
//    if (!rtc.begin()) {
//      Serial.println("Couldn't find RTC. Will continue with system time.");
//    } else {
//      Serial.println("RTC initialized");

//      // Check if RTC lost power
//      if (rtc.lostPower()) {
//        Serial.println("RTC lost power, setting time to compile time");
//        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//      }

//      // Print current time
//      DateTime now = rtc.now();
//      Serial.print("Current time: ");
//      Serial.print(now.hour());
//      Serial.print(":");
//      Serial.print(now.minute());
//      Serial.print(":");
//      Serial.println(now.second());
//    }

//    // Initialize GSM module
//    gsm.begin(9600);
//    Serial.println("GSM module initialized");

//    // Set up WiFi Access Point
//    WiFi.softAP(ssid, password);
//    delay(100);
//    IPAddress IP = WiFi.softAPIP();
//    Serial.print("AP IP address: ");
//    Serial.println(IP);
//    Serial.print("WiFi network name: ");
//    Serial.println(ssid);
//    Serial.print("WiFi password: ");
//    Serial.println(password);

//    // Load users and schedules
//    loadUsers(users, numUsers);
//    loadSchedules(schedules, numSchedules);
//    patientPhone = loadPatientPhone();

//    // Print loaded data
//    Serial.print("Loaded ");
//    Serial.print(numUsers);
//    Serial.println(" users");
//    Serial.print("Loaded ");
//    Serial.print(numSchedules);
//    Serial.println(" schedules");
//    if (patientPhone.length() > 0) {
//      Serial.print("Patient phone: ");
//      Serial.println(patientPhone);
//    } else {
//      Serial.println("No patient phone set");
//    }

//    // Set up web server routes
//    setupWebServer(server, schedules, numSchedules, users, numUsers, patientPhone);

//    // Start server
//    server.begin();
//    Serial.println("Web server started");

//    // Flash all LEDs to indicate startup
//    for (int i = 0; i < 3; i++) {
//      for (int j = 0; j < NUM_CHAMBERS; j++) {
//        digitalWrite(LED_PINS[j], HIGH);
//      }
//      delay(300);
//      for (int j = 0; j < NUM_CHAMBERS; j++) {
//        digitalWrite(LED_PINS[j], LOW);
//      }
//      delay(300);
//    }

//    Serial.println("Smart Medicine Reminder Kit is ready!");
//    Serial.println("Connect to WiFi network and go to 192.168.4.1");
//    Serial.println("Default login: username 'admin', password 'admin'");
//  }

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n\nStarting Smart Medicine Reminder Kit...");

    // Initialize pins
    for (int i = 0; i < NUM_CHAMBERS; i++) {
      pinMode(LED_PINS[i], OUTPUT);
      digitalWrite(LED_PINS[i], LOW);
    }
    pinMode(REED_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    // Initialize LittleFS
    if (!LittleFS.begin(false)) {
      Serial.println("An error occurred while mounting LittleFS. Trying to format...");
      if (!LittleFS.begin(true)) {
        Serial.println("LittleFS mount failed even after formatting");
      } else {
        Serial.println("LittleFS formatted and mounted successfully");
      }
    } else {
      Serial.println("LittleFS mounted successfully");
    }

    // Initialize I2C for RTC - only once
    Wire.begin();
    delay(100);

    // Check for RTC once, don't repeatedly initialize
    if (rtc.begin()) {
      Serial.println("RTC found and initialized");

      // Check if RTC lost power
      if (rtc.lostPower()) {
        Serial.println("RTC lost power, setting time to compile time");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      }

      // Print current time
      DateTime now = rtc.now();
      Serial.print("Current time: ");
      Serial.print(now.hour());
      Serial.print(":");
      Serial.print(now.minute());
      Serial.print(":");
      Serial.println(now.second());
    } else {
      Serial.println("Couldn't find RTC. Will use system time instead.");
    }

    // Initialize GSM module
    gsm.begin(9600);
    Serial.println("GSM module initialized");

    // Set up WiFi Access Point
    WiFi.softAP(ssid, password);
    delay(100);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    Serial.print("WiFi network name: ");
    Serial.println(ssid);
    Serial.print("WiFi password: ");
    Serial.println(password);

    // Load users and schedules
    loadUsers(users, numUsers);
    loadSchedules(schedules, numSchedules);
    patientPhone = loadPatientPhone();

    // Print loaded data
    Serial.print("Loaded ");
    Serial.print(numUsers);
    Serial.println(" users");
    Serial.print("Loaded ");
    Serial.print(numSchedules);
    Serial.println(" schedules");
    if (patientPhone.length() > 0) {
      Serial.print("Patient phone: ");
      Serial.println(patientPhone);
    } else {
      Serial.println("No patient phone set");
    }

    // Set up web server routes
    setupWebServer(server, schedules, numSchedules, users, numUsers, patientPhone);

    // Start server
    server.begin();
    Serial.println("Web server started");

    // Flash all LEDs to indicate startup
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < NUM_CHAMBERS; j++) {
        digitalWrite(LED_PINS[j], HIGH);
      }
      delay(300);
      for (int j = 0; j < NUM_CHAMBERS; j++) {
        digitalWrite(LED_PINS[j], LOW);
      }
      delay(300);
    }

    Serial.println("Smart Medicine Reminder Kit is ready!");
    Serial.println("Connect to WiFi network and go to 192.168.4.1");
    Serial.println("Default login: username 'admin', password 'admin'");
  }

 void loop() {
   // Check medicine schedules
   checkSchedules();

   // Check if medicine is taken
   checkReedSwitches();

   delay(100);
 }

 // Check if medicine is taken by monitoring reed switch
 void checkReedSwitches() {
   // Read the reed switch state (HIGH when box is opened)
   bool boxOpened = (digitalRead(REED_PIN) == HIGH);

   if (boxOpened) {
    //  Serial.println("Medicine box opened detected!");

     // If any box is opened, check which LED is currently active
     for (int i = 0; i < numSchedules; i++) {
       if (schedules[i].alarmActive && !schedules[i].taken) {
         int chamberIndex = schedules[i].chamberIndex;

         if (chamberIndex >= 0 && chamberIndex < NUM_CHAMBERS) {
           // Since we can't know which specific chamber was opened,
           // we assume the active one was opened
           schedules[i].taken = true;
           schedules[i].alarmActive = false;

           // Turn off the LED for this chamber
           digitalWrite(LED_PINS[chamberIndex], LOW);

           // Turn off the buzzer
           digitalWrite(BUZZER_PIN, LOW);

           Serial.print("Medicine taken for schedule ");
           Serial.println(i);

           // Save updated schedules
           saveSchedules(schedules, numSchedules);
         }
       }
     }
   }
 }

 // Check schedules and activate alarms
//  void checkSchedules() {
//    DateTime now;
//    int currentHour = 0;
//    int currentMinute = 0;

//    // Get current time from RTC if available
//    if (rtc.begin()) {
//      now = rtc.now();
//      currentHour = now.hour();
//      currentMinute = now.minute();
//    } else {
//      // Use millis() to create a fake clock for testing
//      static unsigned long startMillis = millis();
//      unsigned long elapsedSeconds = (millis() - startMillis) / 1000;

//      // Convert to hours:minutes (1 real second = 1 virtual minute for testing)
//      currentHour = (elapsedSeconds / 60) % 24;
//      currentMinute = elapsedSeconds % 60;

//      // Display fake time every minute for debugging
//      static int lastMinute = -1;
//      if (lastMinute != currentMinute) {
//        lastMinute = currentMinute;
//        Serial.print("Testing time: ");
//        Serial.print(currentHour);
//        Serial.print(":");
//        Serial.println(currentMinute);
//      }
//    }

//    // Check each schedule
//    for (int i = 0; i < numSchedules; i++) {
//      // If the schedule matches current time and not already taken
//      if (schedules[i].hour == currentHour &&
//          schedules[i].minute == currentMinute &&
//          !schedules[i].taken &&
//          !schedules[i].alarmActive) {

//        // Activate the alarm
//        schedules[i].alarmActive = true;
//        schedules[i].alarmStartTime = millis();

//        // Turn on the LED for the specific chamber
//        if (schedules[i].chamberIndex >= 0 && schedules[i].chamberIndex < NUM_CHAMBERS) {
//          digitalWrite(LED_PINS[schedules[i].chamberIndex], HIGH);
//        }

//        // Sound the buzzer
//        digitalWrite(BUZZER_PIN, HIGH);

//        Serial.print("Alarm activated for chamber ");
//        Serial.print(schedules[i].chamberIndex + 1);
//        Serial.print(" at time ");
//        Serial.print(currentHour);
//        Serial.print(":");
//        Serial.println(currentMinute);
//      }

//      // If alarm is active but medicine not taken
//      if (schedules[i].alarmActive && !schedules[i].taken) {
//        // Check if alarm duration has elapsed (turn off buzzer after 30 seconds)
//        if ((millis() - schedules[i].alarmStartTime > ALARM_DURATION_MS) &&
//             digitalRead(BUZZER_PIN) == HIGH) {
//          digitalWrite(BUZZER_PIN, LOW);
//          Serial.println("Alarm sound stopped, but LED remains on");
//        }

//        // Check if we should make a phone call (after 5 minutes)
//        if (!schedules[i].callSent &&
//            (millis() - schedules[i].alarmStartTime > CALL_DELAY_MS)) {

//          // If patient phone is set, make the call
//          if (patientPhone.length() > 0) {
//            Serial.print("Making reminder call to ");
//            Serial.println(patientPhone);

//            gsm.makeCall(patientPhone);
//            schedules[i].callSent = true;

//            // Save updated schedules
//            saveSchedules(schedules, numSchedules);
//          } else {
//            Serial.println("No patient phone number set, can't make reminder call");
//            schedules[i].callSent = true; // Mark as sent anyway to avoid repeated checks
//          }
//        }
//      }
//    }
//  }


// Replace the current checkSchedules() function with this improved version
void checkSchedules() {
    static bool rtcInitialized = false;
    static bool rtcAvailable = false;

    int currentHour = 0;
    int currentMinute = 0;

    // Initialize RTC only once
    if (!rtcInitialized) {
      rtcInitialized = true;
      rtcAvailable = rtc.begin();

      if (rtcAvailable) {
        Serial.println("RTC initialized successfully");
      } else {
        Serial.println("RTC not available, using system time instead");
      }
    }

    // Get current time
    if (rtcAvailable) {
      // Use RTC time
      DateTime now = rtc.now();
      currentHour = now.hour();
      currentMinute = now.minute();
    } else {
      // Use millis() to create a fake clock for testing
      static unsigned long startMillis = millis();
      unsigned long elapsedSeconds = (millis() - startMillis) / 1000;

      // Convert to hours:minutes (1 real second = 1 virtual minute for testing)
      currentHour = (elapsedSeconds / 60) % 24;
      currentMinute = elapsedSeconds % 60;

      // Display fake time every minute for debugging
      static int lastMinute = -1;
      if (lastMinute != currentMinute) {
        lastMinute = currentMinute;
        Serial.print("Testing time: ");
        Serial.print(currentHour);
        Serial.print(":");
        Serial.println(currentMinute);
      }
    }

    // Check each schedule
    for (int i = 0; i < numSchedules; i++) {
      // If the schedule matches current time and not already taken
      if (schedules[i].hour == currentHour &&
          schedules[i].minute == currentMinute &&
          !schedules[i].taken &&
          !schedules[i].alarmActive) {

        // Activate the alarm
        schedules[i].alarmActive = true;
        schedules[i].alarmStartTime = millis();

        // Turn on the LED for the specific chamber
        if (schedules[i].chamberIndex >= 0 && schedules[i].chamberIndex < NUM_CHAMBERS) {
          digitalWrite(LED_PINS[schedules[i].chamberIndex], HIGH);
        }

        // Sound the buzzer
        digitalWrite(BUZZER_PIN, HIGH);

        Serial.print("Alarm activated for chamber ");
        Serial.print(schedules[i].chamberIndex + 1);
        Serial.print(" at time ");
        Serial.print(currentHour);
        Serial.print(":");
        Serial.println(currentMinute);
      }

      // If alarm is active but medicine not taken
      if (schedules[i].alarmActive && !schedules[i].taken) {
        // Check if alarm duration has elapsed (turn off buzzer after 30 seconds)
        if ((millis() - schedules[i].alarmStartTime > ALARM_DURATION_MS) &&
             digitalRead(BUZZER_PIN) == HIGH) {
          digitalWrite(BUZZER_PIN, LOW);
          Serial.println("Alarm sound stopped, but LED remains on");
        }

        // Check if we should make a phone call (after 5 minutes)
        if (!schedules[i].callSent &&
            (millis() - schedules[i].alarmStartTime > CALL_DELAY_MS)) {

          // If patient phone is set, make the call
          if (patientPhone.length() > 0) {
            Serial.print("Making reminder call to ");
            Serial.println(patientPhone);

            gsm.makeCall(patientPhone);
            schedules[i].callSent = true;

            // Save updated schedules
            saveSchedules(schedules, numSchedules);
          } else {
            Serial.println("No patient phone number set, can't make reminder call");
            schedules[i].callSent = true; // Mark as sent anyway to avoid repeated checks
          }
        }
      }
    }
  }