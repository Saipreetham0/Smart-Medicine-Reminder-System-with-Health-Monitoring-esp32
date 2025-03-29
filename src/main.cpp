

// #include <Arduino.h>
// #include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Wire.h>
// #include <RTClib.h>
// #include <SoftwareSerial.h>
// #include <LittleFS.h>

// // Include project headers
// #include "data_structures.h"
// #include "file_storage.h"
// #include "html_assets.h"
// #include "web_server.h"

// // Pin Definitions for Medicine Chambers
// #define LED_1 13
// #define LED_2 12
// #define LED_3 14
// #define LED_4 27
// #define LED_5 26
// #define LED_6 25
// #define LED_7 33
// #define LED_8 32
// const int LED_PINS[] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8};
// const int NUM_CHAMBERS = 8;

// // Reed Switch Pin (all reed switches connected to this pin)
// #define REED_PIN 19

// // Pin for Buzzer
// #define BUZZER_PIN 5

// // GSM Module pins
// #define GSM_RX 16
// #define GSM_TX 17

// // WiFi Access Point Settings
// const char *ssid = "Medicine Reminder";
// const char *password = "medkit123";

// // Web Server
// AsyncWebServer server(80);

// // RTC Module
// RTC_DS3231 rtc;

// // GSM Module setup
// SoftwareSerial gsmSerial(GSM_RX, GSM_TX);

// // Variables for timing
// const int CALL_DELAY_MS = 300000;    // 5 minutes (300,000 ms)
// const int ALARM_DURATION_MS = 30000; // 30 seconds

// // Function declarations
// void checkSchedules();
// void checkReedSwitches();
// void makeCall(const String &phoneNumber);

// void setup()
// {
//     Serial.begin(115200);
//     delay(1000);
//     Serial.println("\n\nStarting Smart Medicine Reminder Kit...");

//     // Initialize pins
//     for (int i = 0; i < NUM_CHAMBERS; i++)
//     {
//         pinMode(LED_PINS[i], OUTPUT);
//         digitalWrite(LED_PINS[i], LOW);
//     }
//     pinMode(REED_PIN, INPUT_PULLUP);
//     pinMode(BUZZER_PIN, OUTPUT);
//     digitalWrite(BUZZER_PIN, LOW);

//     // Initialize filesystem
//     if (!initFileSystem())
//     {
//         Serial.println("Failed to initialize filesystem, attempting format...");
//         if (LittleFS.format())
//         {
//             Serial.println("Filesystem formatted successfully");
//             if (!initFileSystem())
//             {
//                 Serial.println("Still failed to initialize filesystem after formatting");
//             }
//         }
//         else
//         {
//             Serial.println("Failed to format filesystem");
//         }
//     }

//     // Initialize GSM module
//     gsmSerial.begin(9600);
//     delay(1000);

//     // Initialize GSM with basic AT commands
//     gsmSerial.println("AT");
//     delay(1000);
//     gsmSerial.println("AT+CMGF=1");
//     delay(1000);
//     gsmSerial.println("AT+CNMI=1,2,0,0,0");
//     delay(1000);
//     Serial.println("GSM module initialized");

//     // Initialize I2C for RTC - only once
//     Wire.begin();
//     delay(100);

//     // Initialize RTC
//     bool rtcFound = false;
//     for (int i = 0; i < 3; i++)
//     { // Try 3 times to initialize RTC
//         if (rtc.begin())
//         {
//             rtcFound = true;
//             Serial.println("RTC found and initialized");

//             // Check if RTC lost power
//             if (rtc.lostPower())
//             {
//                 Serial.println("RTC lost power, setting time to compile time");
//                 rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//             }

//             // Print current time
//             DateTime now = rtc.now();
//             Serial.print("Current time: ");
//             Serial.print(now.hour());
//             Serial.print(":");
//             Serial.print(now.minute());
//             Serial.print(":");
//             Serial.println(now.second());
//             break;
//         }
//         delay(200);
//     }

//     if (!rtcFound)
//     {
//         Serial.println("RTC not found. Will use system time.");
//     }

//     // Set up WiFi Access Point
//     WiFi.softAP(ssid, password);
//     delay(100);
//     IPAddress IP = WiFi.softAPIP();
//     Serial.print("AP IP address: ");
//     Serial.println(IP);
//     Serial.print("WiFi network name: ");
//     Serial.println(ssid);
//     Serial.print("WiFi password: ");
//     Serial.println(password);

//     // Set up web server routes
//     setupWebServer(server);

//     // Start server
//     server.begin();
//     Serial.println("Web server started");

//     // Print initial schedules for debugging
//     printAllSchedules();

//     // Flash all LEDs to indicate startup
//     for (int i = 0; i < 3; i++)
//     {
//         for (int j = 0; j < NUM_CHAMBERS; j++)
//         {
//             digitalWrite(LED_PINS[j], HIGH);
//         }
//         delay(300);
//         for (int j = 0; j < NUM_CHAMBERS; j++)
//         {
//             digitalWrite(LED_PINS[j], LOW);
//         }
//         delay(300);
//     }

//     Serial.println("Smart Medicine Reminder Kit is ready!");
//     Serial.println("Connect to WiFi network and go to 192.168.4.1");
// }

// void loop()
// {
//     // Check medicine schedules
//     checkSchedules();

//     // Check if medicine is taken
//     checkReedSwitches();

//     delay(100);
// }

// void checkReedSwitches()
// {
//     // Read the reed switch state (HIGH when box is opened)
//     bool boxOpened = (digitalRead(REED_PIN) == HIGH);

//     if (boxOpened)
//     {
//         // Serial.println("Medicine box opened detected!");

//         // Check which medicine schedules are active
//         for (int i = 0; i < g_numSchedules; i++)
//         {
//             if (g_schedules[i].alarmActive && !g_schedules[i].taken)
//             {
//                 int chamberIndex = g_schedules[i].chamberIndex;

//                 if (chamberIndex >= 0 && chamberIndex < NUM_CHAMBERS)
//                 {
//                     // Mark as taken
//                     g_schedules[i].taken = true;
//                     g_schedules[i].alarmActive = false;

//                     // Turn off the LED for this chamber
//                     digitalWrite(LED_PINS[chamberIndex], LOW);

//                     // Turn off the buzzer
//                     digitalWrite(BUZZER_PIN, LOW);

//                     Serial.print("Medicine taken for schedule ");
//                     Serial.println(i);

//                     // Save the updated status to persistent storage
//                     updateScheduleStatus(i, true);
//                 }
//             }
//         }
//     }
// }

// void checkSchedules()
// {
//     static bool rtcInitialized = false;
//     static bool rtcAvailable = false;

//     // Initialize RTC check once
//     if (!rtcInitialized)
//     {
//         rtcInitialized = true;
//         rtcAvailable = rtc.begin();
//     }

//     int currentHour = 0;
//     int currentMinute = 0;

//     // Get current time
//     if (rtcAvailable)
//     {
//         // Use RTC time
//         DateTime now = rtc.now();
//         currentHour = now.hour();
//         currentMinute = now.minute();
//     }
//     else
//     {
//         // Use millis() to create a fake clock for testing
//         static unsigned long startMillis = millis();
//         unsigned long elapsedSeconds = (millis() - startMillis) / 1000;

//         // Convert to hours:minutes (1 real second = 1 virtual minute for testing)
//         currentHour = (elapsedSeconds / 60) % 24;
//         currentMinute = elapsedSeconds % 60;

//         // Display fake time every minute for debugging
//         static int lastMinute = -1;
//         if (lastMinute != currentMinute)
//         {
//             lastMinute = currentMinute;
//             Serial.print("Testing time: ");
//             Serial.print(currentHour);
//             Serial.print(":");
//             Serial.println(currentMinute);
//         }
//     }

//     // Check each schedule
//     for (int i = 0; i < g_numSchedules; i++)
//     {
//         // If the schedule matches current time and not already taken
//         if (g_schedules[i].hour == currentHour &&
//             g_schedules[i].minute == currentMinute &&
//             !g_schedules[i].taken &&
//             !g_schedules[i].alarmActive)
//         {

//             // Activate the alarm
//             g_schedules[i].alarmActive = true;
//             g_schedules[i].alarmStartTime = millis();

//             // Turn on the LED for the specific chamber
//             if (g_schedules[i].chamberIndex >= 0 && g_schedules[i].chamberIndex < NUM_CHAMBERS)
//             {
//                 digitalWrite(LED_PINS[g_schedules[i].chamberIndex], HIGH);
//             }

//             // Sound the buzzer
//             digitalWrite(BUZZER_PIN, HIGH);

//             Serial.print("Alarm activated for chamber ");
//             Serial.print(g_schedules[i].chamberIndex + 1);
//             Serial.print(" at time ");
//             Serial.print(currentHour);
//             Serial.print(":");
//             Serial.println(currentMinute);
//         }

//         // If alarm is active but medicine not taken
//         if (g_schedules[i].alarmActive && !g_schedules[i].taken)
//         {
//             // Check if alarm duration has elapsed (turn off buzzer after 30 seconds)
//             if ((millis() - g_schedules[i].alarmStartTime > ALARM_DURATION_MS) &&
//                 digitalRead(BUZZER_PIN) == HIGH)
//             {
//                 digitalWrite(BUZZER_PIN, LOW);
//                 Serial.println("Alarm sound stopped, but LED remains on");
//             }

//             // Check if we should make a phone call (after 5 minutes)
//             if (!g_schedules[i].callSent &&
//                 (millis() - g_schedules[i].alarmStartTime > CALL_DELAY_MS))
//             {

//                 // If patient phone is set, make the call
//                 if (g_patientPhone.length() > 0)
//                 {
//                     Serial.print("Making reminder call to ");
//                     Serial.println(g_patientPhone);

//                     makeCall(g_patientPhone);
//                     g_schedules[i].callSent = true;
//                 }
//                 else
//                 {
//                     Serial.println("No patient phone number set, can't make reminder call");
//                     g_schedules[i].callSent = true; // Mark as sent anyway to avoid repeated checks
//                 }
//             }
//         }
//     }
// }

// // Make a phone call using GSM module
// void makeCall(const String &phoneNumber)
// {
//     // Send AT command to dial
//     String dialCommand = "ATD" + phoneNumber + ";";
//     gsmSerial.println(dialCommand);

//     Serial.println("Dialing...");
//     delay(20000); // Wait for 20 seconds for call to ring

//     // Hang up
//     gsmSerial.println("ATH");
//     Serial.println("Call ended");
// }


#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <LittleFS.h>

// Include project headers
#include "data_structures.h"
#include "file_storage.h"
#include "html_assets.h"
#include "web_server.h"

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

// GSM Module setup
SoftwareSerial gsmSerial(GSM_RX, GSM_TX);

// Variables for timing
const int CALL_DELAY_MS = 60000; // 1 minute (60,000 ms)
const int ALARM_DURATION_MS = 30000; // 30 seconds

// Function declarations
void checkSchedules();
void checkReedSwitches();
void makeCall(const String &phoneNumber);

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

  // Initialize filesystem
  if (!initFileSystem()) {
    Serial.println("Failed to initialize filesystem, attempting format...");
    if (LittleFS.format()) {
      Serial.println("Filesystem formatted successfully");
      if (!initFileSystem()) {
        Serial.println("Still failed to initialize filesystem after formatting");
      }
    } else {
      Serial.println("Failed to format filesystem");
    }
  }

  // Initialize GSM module
  gsmSerial.begin(9600);
  delay(1000);

  // Initialize GSM with basic AT commands
  gsmSerial.println("AT");
  delay(1000);
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CNMI=1,2,0,0,0");
  delay(1000);
  Serial.println("GSM module initialized");

  // Initialize I2C for RTC - only once
  Wire.begin();
  delay(100);

  // Initialize RTC
  bool rtcFound = false;
  for (int i = 0; i < 3; i++) { // Try 3 times to initialize RTC
    if (rtc.begin()) {
      rtcFound = true;
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
      break;
    }
    delay(200);
  }

  if (!rtcFound) {
    Serial.println("RTC not found. Will use system time.");
  }

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

  // Set up web server routes
  setupWebServer(server);

  // Start server
  server.begin();
  Serial.println("Web server started");

  // Print initial schedules for debugging
  printAllSchedules();

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
}

void loop() {
  // Check medicine schedules
  checkSchedules();

  // Check if medicine is taken
  checkReedSwitches();

  delay(100);
}

void checkReedSwitches() {
  // Read the reed switch state (HIGH when box is opened)
  bool boxOpened = (digitalRead(REED_PIN) == HIGH);

  if (boxOpened) {
    Serial.println("Medicine box opened detected!");

    // Check which medicine schedules are active
    for (int i = 0; i < g_numSchedules; i++) {
      if (g_schedules[i].alarmActive && !g_schedules[i].taken) {
        int chamberIndex = g_schedules[i].chamberIndex;

        if (chamberIndex >= 0 && chamberIndex < NUM_CHAMBERS) {
          // Mark as taken
          g_schedules[i].taken = true;
          g_schedules[i].alarmActive = false;

          // Turn off the LED for this chamber
          digitalWrite(LED_PINS[chamberIndex], LOW);

          // Turn off the buzzer
          digitalWrite(BUZZER_PIN, LOW);

          Serial.print("Medicine taken for schedule ");
          Serial.println(i);

          // Save the updated status to persistent storage
          updateScheduleStatus(i, true);
        }
      }
    }
  }
}

void checkSchedules() {
  static bool rtcInitialized = false;
  static bool rtcAvailable = false;

  // Initialize RTC check once
  if (!rtcInitialized) {
    rtcInitialized = true;
    rtcAvailable = rtc.begin();
  }

  int currentHour = 0;
  int currentMinute = 0;

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
  for (int i = 0; i < g_numSchedules; i++) {
    // If the schedule matches current time and not already taken
    if (g_schedules[i].hour == currentHour &&
        g_schedules[i].minute == currentMinute &&
        !g_schedules[i].taken &&
        !g_schedules[i].alarmActive) {

      // Activate the alarm
      g_schedules[i].alarmActive = true;
      g_schedules[i].alarmStartTime = millis();

      // Turn on the LED for the specific chamber
      if (g_schedules[i].chamberIndex >= 0 && g_schedules[i].chamberIndex < NUM_CHAMBERS) {
        digitalWrite(LED_PINS[g_schedules[i].chamberIndex], HIGH);
      }

      // Sound the buzzer
      digitalWrite(BUZZER_PIN, HIGH);

      Serial.print("Alarm activated for chamber ");
      Serial.print(g_schedules[i].chamberIndex + 1);
      Serial.print(" at time ");
      Serial.print(currentHour);
      Serial.print(":");
      Serial.println(currentMinute);
    }

    // If alarm is active but medicine not taken
    if (g_schedules[i].alarmActive && !g_schedules[i].taken) {
      // Check if alarm duration has elapsed (turn off buzzer after 30 seconds)
      if ((millis() - g_schedules[i].alarmStartTime > ALARM_DURATION_MS) &&
           digitalRead(BUZZER_PIN) == HIGH) {
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("Alarm sound stopped, but LED remains on");
      }

      // Check if we should make a phone call (after 1 minute)
      if (!g_schedules[i].callSent &&
          (millis() - g_schedules[i].alarmStartTime > CALL_DELAY_MS)) {

        // If patient phone is set, make the call
        if (g_patientPhone.length() > 0) {
          Serial.print("Making reminder call to ");
          Serial.println(g_patientPhone);

          makeCall(g_patientPhone);
          g_schedules[i].callSent = true;
        } else {
          Serial.println("No patient phone number set, can't make reminder call");
          g_schedules[i].callSent = true; // Mark as sent anyway to avoid repeated checks
        }
      }
    }
  }
}

// Make a phone call using GSM module
void makeCall(const String &phoneNumber) {
  // Send AT command to dial
  String dialCommand = "ATD" + phoneNumber + ";";
  gsmSerial.println(dialCommand);

  Serial.println("Dialing...");
  delay(20000); // Wait for 20 seconds for call to ring

  // Hang up
  gsmSerial.println("ATH");
  Serial.println("Call ended");
}