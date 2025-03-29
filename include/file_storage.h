#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "data_structures.h"

// Global data storage
MedicineSchedule g_schedules[20]; // Store up to 20 medicine schedules
int g_numSchedules = 0;
String g_patientPhone = "";

// Initialize and format filesystem if needed
bool initFileSystem() {
  if (!LittleFS.begin(false)) {
    Serial.println("Failed to mount filesystem, attempting to format...");
    if (!LittleFS.format()) {
      Serial.println("Failed to format filesystem");
      return false;
    }

    if (!LittleFS.begin()) {
      Serial.println("Failed to mount filesystem even after formatting");
      return false;
    }

    Serial.println("Filesystem formatted and mounted successfully");
  } else {
    Serial.println("Filesystem mounted successfully");
  }

  return true;
}

// Load all data from filesystem
void loadAllData() {
  // Reset to default values
  g_numSchedules = 0;
  g_patientPhone = "";

  // Initialize filesystem
  if (!initFileSystem()) {
    return;
  }

  // Load schedules
  if (LittleFS.exists("/schedules.json")) {
    File file = LittleFS.open("/schedules.json", "r");
    if (file) {
      // Read file content
      String jsonStr = file.readString();
      file.close();

      // Parse JSON
      DynamicJsonDocument doc(4096);
      DeserializationError error = deserializeJson(doc, jsonStr);

      if (!error) {
        if (doc.containsKey("schedules") && doc["schedules"].is<JsonArray>()) {
          JsonArray schedulesArray = doc["schedules"].as<JsonArray>();
          g_numSchedules = 0;

          for (JsonObject scheduleObj : schedulesArray) {
            if (g_numSchedules < 20) {
              g_schedules[g_numSchedules].chamberIndex = scheduleObj["chamber"].as<int>();
              g_schedules[g_numSchedules].hour = scheduleObj["hour"].as<int>();
              g_schedules[g_numSchedules].minute = scheduleObj["minute"].as<int>();
              g_schedules[g_numSchedules].taken = scheduleObj["taken"].as<bool>();
              g_schedules[g_numSchedules].alarmActive = false; // Always start with alarm inactive
              g_schedules[g_numSchedules].callSent = false;     // Reset call status
              g_numSchedules++;
            }
          }

          Serial.print("Loaded ");
          Serial.print(g_numSchedules);
          Serial.println(" schedules");
        }
      } else {
        Serial.print("Error parsing schedules JSON: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.println("Failed to open schedules.json for reading");
    }
  } else {
    Serial.println("No schedules file found");
  }

  // Load phone number
  if (LittleFS.exists("/phone.txt")) {
    File file = LittleFS.open("/phone.txt", "r");
    if (file) {
      g_patientPhone = file.readString();
      file.close();

      Serial.print("Loaded phone number: ");
      Serial.println(g_patientPhone);
    } else {
      Serial.println("Failed to open phone.txt for reading");
    }
  } else {
    Serial.println("No phone file found");
  }
}

// Save all schedules to file
bool saveSchedules() {
  if (!initFileSystem()) {
    return false;
  }

  // Create JSON document
  DynamicJsonDocument doc(4096);
  JsonArray schedulesArray = doc.createNestedArray("schedules");

  for (int i = 0; i < g_numSchedules; i++) {
    JsonObject scheduleObj = schedulesArray.createNestedObject();
    scheduleObj["chamber"] = g_schedules[i].chamberIndex;
    scheduleObj["hour"] = g_schedules[i].hour;
    scheduleObj["minute"] = g_schedules[i].minute;
    scheduleObj["taken"] = g_schedules[i].taken;
  }

  // Serialize to String first (for debugging)
  String jsonStr;
  serializeJson(doc, jsonStr);
  Serial.print("Saving schedules JSON: ");
  Serial.println(jsonStr);

  // Write to file
  File file = LittleFS.open("/schedules.json", "w");
  if (!file) {
    Serial.println("Failed to open schedules.json for writing");
    return false;
  }

  size_t bytesWritten = serializeJson(doc, file);
  file.close();

  if (bytesWritten == 0) {
    Serial.println("Failed to write to schedules.json");
    return false;
  }

  Serial.print("Saved ");
  Serial.print(g_numSchedules);
  Serial.println(" schedules");
  return true;
}

// Save phone number to file
bool savePhone() {
  if (!initFileSystem()) {
    return false;
  }

  File file = LittleFS.open("/phone.txt", "w");
  if (!file) {
    Serial.println("Failed to open phone.txt for writing");
    return false;
  }

  size_t bytesWritten = file.print(g_patientPhone);
  file.close();

  if (bytesWritten == 0) {
    Serial.println("Failed to write to phone.txt");
    return false;
  }

  Serial.print("Saved phone number: ");
  Serial.println(g_patientPhone);
  return true;
}

// Add a schedule
bool addSchedule(int chamber, int hour, int minute) {
  if (g_numSchedules >= 20) {
    Serial.println("ERROR: Maximum number of schedules reached");
    return false;
  }

  g_schedules[g_numSchedules].chamberIndex = chamber;
  g_schedules[g_numSchedules].hour = hour;
  g_schedules[g_numSchedules].minute = minute;
  g_schedules[g_numSchedules].taken = false;
  g_schedules[g_numSchedules].alarmActive = false;
  g_schedules[g_numSchedules].callSent = false;
  g_numSchedules++;

  // Save to file
  if (!saveSchedules()) {
    // If save fails, undo the addition
    g_numSchedules--;
    return false;
  }

  Serial.print("Schedule added. Chamber: ");
  Serial.print(chamber);
  Serial.print(", Time: ");
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);
  Serial.print("Total schedules: ");
  Serial.println(g_numSchedules);

  return true;
}

// Clear all schedules
bool clearSchedules() {
  g_numSchedules = 0;

  // Save empty schedule list
  if (!saveSchedules()) {
    return false;
  }

  Serial.println("All schedules cleared");
  return true;
}

// Remove a schedule by index
bool removeSchedule(int index) {
  if (index < 0 || index >= g_numSchedules) {
    Serial.println("ERROR: Invalid schedule index");
    return false;
  }

  // Shift all schedules down
  for (int i = index; i < g_numSchedules - 1; i++) {
    g_schedules[i] = g_schedules[i + 1];
  }

  g_numSchedules--;

  // Save to file
  if (!saveSchedules()) {
    return false;
  }

  Serial.print("Schedule removed. New count: ");
  Serial.println(g_numSchedules);

  return true;
}

// Set patient phone
bool setPatientPhone(const String &phone) {
  g_patientPhone = phone;

  // Save to file
  if (!savePhone()) {
    return false;
  }

  Serial.print("Patient phone set to: ");
  Serial.println(phone);

  return true;
}

// Update schedule status (taken, etc.)
bool updateScheduleStatus(int index, bool taken) {
  if (index >= 0 && index < g_numSchedules) {
    g_schedules[index].taken = taken;

    // Save to file
    if (!saveSchedules()) {
      return false;
    }

    Serial.print("Schedule #");
    Serial.print(index);
    Serial.print(" status updated: taken=");
    Serial.println(taken ? "yes" : "no");

    return true;
  }

  return false;
}

// Get all schedules as JSON
String getSchedulesJson() {
  DynamicJsonDocument doc(4096);
  JsonArray schedulesArray = doc.createNestedArray("schedules");

  for (int i = 0; i < g_numSchedules; i++) {
    JsonObject scheduleObj = schedulesArray.createNestedObject();
    scheduleObj["chamber"] = g_schedules[i].chamberIndex;
    scheduleObj["hour"] = g_schedules[i].hour;
    scheduleObj["minute"] = g_schedules[i].minute;
    scheduleObj["taken"] = g_schedules[i].taken;
  }

  String jsonStr;
  serializeJson(doc, jsonStr);

  Serial.print("JSON generated for ");
  Serial.print(g_numSchedules);
  Serial.println(" schedules");

  return jsonStr;
}

// Print all schedules to Serial for debugging
void printAllSchedules() {
  Serial.println("===== CURRENT SCHEDULES =====");
  Serial.print("Number of schedules: ");
  Serial.println(g_numSchedules);

  for (int i = 0; i < g_numSchedules; i++) {
    Serial.print("Schedule #");
    Serial.print(i);
    Serial.print(": Chamber ");
    Serial.print(g_schedules[i].chamberIndex + 1);
    Serial.print(", Time ");
    Serial.print(g_schedules[i].hour);
    Serial.print(":");
    Serial.print(g_schedules[i].minute);
    Serial.print(", Taken: ");
    Serial.println(g_schedules[i].taken ? "Yes" : "No");
  }

  Serial.print("Patient Phone: ");
  Serial.println(g_patientPhone);
  Serial.println("=============================");
}

#endif // FILE_STORAGE_H