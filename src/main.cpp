
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <RTClib.h>
#include <LittleFS.h>

// Include project headers
#include "data_structures.h"
#include "file_storage.h"
#include "html_assets.h"
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
#define GSM_RX 17
#define GSM_TX 16

// WiFi Access Point Settings
const char* ssid = "Medicine Reminder";
const char* password = "medkit123";

// Web Server
AsyncWebServer server(80);

// RTC Module
RTC_DS3231 rtc;

// GSM Module setup using Hardware Serial
GSMModule gsm(Serial2);

// Variables for timing
const int CALL_DELAY_MS = 60000; // 1 minute (60,000 ms)
const int ALARM_DURATION_MS = 30000; // 30 seconds

// Global time variables
int g_currentHour = 0;
int g_currentMinute = 0;
int g_currentSecond = 0;
int g_currentDay = 1;
int g_currentMonth = 1;
int g_currentYear = 2023;
String g_currentTimeString = "00:00:00";
String g_currentDateString = "2023-01-01";

// Function declarations
void checkSchedules();
void checkReedSwitches();
void updateCurrentTime();
String getCurrentTimeString();
String getCurrentDateString();
String getTimeAndDateJson();

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
  gsm.begin(115200);  // Start with 115200 baud rate

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

      // Update and print current time
      updateCurrentTime();
      Serial.print("Current time: ");
      Serial.print(g_currentTimeString);
      Serial.print(" | Date: ");
      Serial.println(g_currentDateString);
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

  // Test message (uncomment to test)
  // gsm.sendSMS("+91XXXXXXXXXX", "Smart Medicine Reminder is ready!");
}

void loop() {
  // Update current time
  updateCurrentTime();

  // Check medicine schedules
  checkSchedules();

  // Check if medicine is taken
  checkReedSwitches();

  // Check for GSM module events
  gsm.update();

  delay(100);
}

// Update current time from RTC or system time
void updateCurrentTime() {
  static bool rtcInitialized = false;
  static bool rtcAvailable = false;
  static unsigned long lastTimeUpdate = 0;

  // Don't update time too frequently
  if (millis() - lastTimeUpdate < 1000) {
    return;
  }

  lastTimeUpdate = millis();

  // Initialize RTC check once
  if (!rtcInitialized) {
    rtcInitialized = true;
    rtcAvailable = rtc.begin();
  }

  // Get current time
  if (rtcAvailable) {
    // Use RTC time
    DateTime now = rtc.now();
    g_currentHour = now.hour();
    g_currentMinute = now.minute();
    g_currentSecond = now.second();
    g_currentDay = now.day();
    g_currentMonth = now.month();
    g_currentYear = now.year();
  } else {
    // Use millis() to create a fake clock for testing
    static unsigned long startMillis = millis();
    unsigned long elapsedSeconds = (millis() - startMillis) / 1000;

    // Convert to hours:minutes:seconds
    g_currentHour = (elapsedSeconds / 3600) % 24;
    g_currentMinute = (elapsedSeconds / 60) % 60;
    g_currentSecond = elapsedSeconds % 60;

    // For testing, use a fixed date
    g_currentDay = 1;
    g_currentMonth = 1;
    g_currentYear = 2023;

    // Display fake time every minute for debugging
    static int lastMinute = -1;
    if (lastMinute != g_currentMinute) {
      lastMinute = g_currentMinute;
      Serial.print("Testing time: ");
      Serial.println(getCurrentTimeString());
    }
  }

  // Update formatted time strings
  g_currentTimeString = getCurrentTimeString();
  g_currentDateString = getCurrentDateString();
}

// Format current time as "HH:MM:SS"
String getCurrentTimeString() {
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", g_currentHour, g_currentMinute, g_currentSecond);
  return String(timeStr);
}

// Format current date as "YYYY-MM-DD"
String getCurrentDateString() {
  char dateStr[11];
  sprintf(dateStr, "%04d-%02d-%02d", g_currentYear, g_currentMonth, g_currentDay);
  return String(dateStr);
}

// Get current time and date as JSON
String getTimeAndDateJson() {
  String json = "{\"time\":\"";
  json += getCurrentTimeString();
  json += "\",\"date\":\"";
  json += getCurrentDateString();
  json += "\"}";
  return json;
}

void checkReedSwitches() {
  // Read the reed switch state (HIGH when box is opened)
  bool boxOpened = (digitalRead(REED_PIN) == HIGH);

  if (boxOpened) {
    Serial.print("[");
    Serial.print(g_currentTimeString);
    Serial.println("] Medicine box opened detected!");

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

          Serial.print("[");
          Serial.print(g_currentTimeString);
          Serial.print("] Medicine taken for schedule ");
          Serial.println(i);

          // Save the updated status to persistent storage
          updateScheduleStatus(i, true);
        }
      }
    }
  }
}

void checkSchedules() {
  // Check each schedule
  for (int i = 0; i < g_numSchedules; i++) {
    // If the schedule matches current time and not already taken
    if (g_schedules[i].hour == g_currentHour &&
        g_schedules[i].minute == g_currentMinute &&
        g_currentSecond < 2 &&  // Only trigger in the first 2 seconds of the minute
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

      Serial.print("[");
      Serial.print(g_currentTimeString);
      Serial.print("] Alarm activated for chamber ");
      Serial.print(g_schedules[i].chamberIndex + 1);
      Serial.print(" at time ");
      Serial.print(g_currentHour);
      Serial.print(":");
      Serial.println(g_currentMinute);
    }

    // If alarm is active but medicine not taken
    if (g_schedules[i].alarmActive && !g_schedules[i].taken) {
      // Check if alarm duration has elapsed (turn off buzzer after 30 seconds)
      if ((millis() - g_schedules[i].alarmStartTime > ALARM_DURATION_MS) &&
           digitalRead(BUZZER_PIN) == HIGH) {
        digitalWrite(BUZZER_PIN, LOW);
        Serial.print("[");
        Serial.print(g_currentTimeString);
        Serial.println("] Alarm sound stopped, but LED remains on");
      }

      // Check if we should make a phone call (after 1 minute)
      if (!g_schedules[i].callSent &&
          (millis() - g_schedules[i].alarmStartTime > CALL_DELAY_MS)) {

        // If patient phone is set, make the call
        if (g_patientPhone.length() > 0) {
          Serial.print("[");
          Serial.print(g_currentTimeString);
          Serial.print("] Making reminder call to ");
          Serial.println(g_patientPhone);

          // Call using the GSM module
          gsm.makeCall(g_patientPhone);

          // Also send an SMS reminder
          String message = "Reminder: Please take your medicine from Chamber " +
                           String(g_schedules[i].chamberIndex + 1) +
                           " scheduled for " +
                           String(g_schedules[i].hour) + ":" +
                           String(g_schedules[i].minute);
          gsm.sendSMS(g_patientPhone, message);

          g_schedules[i].callSent = true;
        } else {
          Serial.print("[");
          Serial.print(g_currentTimeString);
          Serial.println("] No patient phone number set, can't make reminder call");
          g_schedules[i].callSent = true; // Mark as sent anyway to avoid repeated checks
        }
      }
    }
  }
}