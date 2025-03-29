// // Include this in a new file called memory_storage.h

// #ifndef MEMORY_STORAGE_H
// #define MEMORY_STORAGE_H

// #include <Arduino.h>
// #include "data_structures.h"

// // Global data storage - no filesystem needed
// MedicineSchedule g_schedules[20]; // Store up to 20 medicine schedules
// int g_numSchedules = 0;
// String g_patientPhone = "";
// User g_users[10];
// int g_numUsers = 0;

// // Initialize default user
// void initDefaultUser() {
//   g_users[0].username = "admin";
//   g_users[0].password = "admin";
//   g_users[0].role = "admin";
//   g_numUsers = 1;

//   Serial.println("Default admin user created");
// }

// // Add a schedule
// bool addSchedule(int chamber, int hour, int minute) {
//   if (g_numSchedules >= 20) {
//     Serial.println("ERROR: Maximum number of schedules reached");
//     return false;
//   }

//   g_schedules[g_numSchedules].chamberIndex = chamber;
//   g_schedules[g_numSchedules].hour = hour;
//   g_schedules[g_numSchedules].minute = minute;
//   g_schedules[g_numSchedules].taken = false;
//   g_schedules[g_numSchedules].alarmActive = false;
//   g_schedules[g_numSchedules].callSent = false;
//   g_numSchedules++;

//   Serial.print("Schedule added. Chamber: ");
//   Serial.print(chamber);
//   Serial.print(", Time: ");
//   Serial.print(hour);
//   Serial.print(":");
//   Serial.println(minute);

//   return true;
// }

// // Clear all schedules
// void clearSchedules() {
//   g_numSchedules = 0;
//   Serial.println("All schedules cleared");
// }

// // Remove a schedule by index
// bool removeSchedule(int index) {
//   if (index < 0 || index >= g_numSchedules) {
//     Serial.println("ERROR: Invalid schedule index");
//     return false;
//   }

//   // Shift all schedules down
//   for (int i = index; i < g_numSchedules - 1; i++) {
//     g_schedules[i] = g_schedules[i + 1];
//   }

//   g_numSchedules--;
//   Serial.print("Schedule removed. New count: ");
//   Serial.println(g_numSchedules);

//   return true;
// }

// // Set patient phone
// void setPatientPhone(const String &phone) {
//   g_patientPhone = phone;
//   Serial.print("Patient phone set to: ");
//   Serial.println(phone);
// }

// // Get all schedules as JSON
// String getSchedulesJson() {
//   String json = "{\"schedules\":[";

//   for (int i = 0; i < g_numSchedules; i++) {
//     if (i > 0) {
//       json += ",";
//     }

//     json += "{\"chamber\":";
//     json += g_schedules[i].chamberIndex;
//     json += ",\"hour\":";
//     json += g_schedules[i].hour;
//     json += ",\"minute\":";
//     json += g_schedules[i].minute;
//     json += ",\"taken\":";
//     json += g_schedules[i].taken ? "true" : "false";
//     json += "}";
//   }

//   json += "]}";
//   return json;
// }

// #endif // MEMORY_STORAGE_H


#ifndef MEMORY_STORAGE_H
#define MEMORY_STORAGE_H

#include <Arduino.h>
#include "data_structures.h"

// Global data storage - no filesystem needed
MedicineSchedule g_schedules[20]; // Store up to 20 medicine schedules
int g_numSchedules = 0;
String g_patientPhone = "";

// Initialize with default phone number for testing
void initStorage() {
  g_patientPhone = ""; // Clear initial value
  g_numSchedules = 0;

  // Uncomment the lines below to add some test schedules
  //addSchedule(0, 8, 0);  // Chamber 1, 8:00
  //addSchedule(1, 12, 30); // Chamber 2, 12:30
  //addSchedule(2, 18, 0);  // Chamber 3, 18:00

  Serial.println("Memory storage initialized");
  Serial.print("Initial schedules: ");
  Serial.println(g_numSchedules);
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
void clearSchedules() {
  g_numSchedules = 0;
  Serial.println("All schedules cleared");
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
  Serial.print("Schedule removed. New count: ");
  Serial.println(g_numSchedules);

  return true;
}

// Set patient phone
void setPatientPhone(const String &phone) {
  g_patientPhone = phone;
  Serial.print("Patient phone set to: ");
  Serial.println(phone);
}

// Get all schedules as JSON
String getSchedulesJson() {
  String json = "{\"schedules\":[";

  for (int i = 0; i < g_numSchedules; i++) {
    if (i > 0) {
      json += ",";
    }

    json += "{\"chamber\":";
    json += g_schedules[i].chamberIndex;
    json += ",\"hour\":";
    json += g_schedules[i].hour;
    json += ",\"minute\":";
    json += g_schedules[i].minute;
    json += ",\"taken\":";
    json += g_schedules[i].taken ? "true" : "false";
    json += "}";
  }

  json += "]}";

  Serial.print("JSON generated for ");
  Serial.print(g_numSchedules);
  Serial.print(" schedules: ");
  Serial.println(json);

  return json;
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

#endif // MEMORY_STORAGE_H