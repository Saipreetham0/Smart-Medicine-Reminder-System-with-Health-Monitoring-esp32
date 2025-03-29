// #ifndef FILE_OPERATIONS_H
// #define FILE_OPERATIONS_H

// #include <Arduino.h>
// #include <LittleFS.h>
// #include <ArduinoJson.h>
// #include "data_structures.h"

// // Function declarations
// void saveSchedules(MedicineSchedule schedules[], int numSchedules);
// void loadSchedules(MedicineSchedule schedules[], int &numSchedules);
// void savePatientPhone(const String &phone);
// String loadPatientPhone();
// void saveUsers(User users[], int numUsers);
// void loadUsers(User users[], int &numUsers);

// // Save schedules to LittleFS
// void saveSchedules(MedicineSchedule schedules[], int numSchedules) {
//   File file = LittleFS.open("/schedules.json", "w");
//   if (!file) {
//     Serial.println("Failed to open schedules file for writing");
//     return;
//   }

//   StaticJsonDocument<1024> doc;
//   JsonArray scheduleArray = doc.createNestedArray("schedules");

//   for (int i = 0; i < numSchedules; i++) {
//     JsonObject schedule = scheduleArray.createNestedObject();
//     schedule["chamber"] = schedules[i].chamberIndex;
//     schedule["hour"] = schedules[i].hour;
//     schedule["minute"] = schedules[i].minute;
//   }

//   serializeJson(doc, file);
//   file.close();
//   Serial.println("Schedules saved to LittleFS");
// }

// // Load schedules from LittleFS
// void loadSchedules(MedicineSchedule schedules[], int &numSchedules) {
//   if (!LittleFS.exists("/schedules.json")) {
//     Serial.println("No schedules file found");
//     numSchedules = 0;
//     return;
//   }

//   File file = LittleFS.open("/schedules.json", "r");
//   if (!file) {
//     Serial.println("Failed to open schedules file");
//     numSchedules = 0;
//     return;
//   }

//   StaticJsonDocument<1024> doc;
//   DeserializationError error = deserializeJson(doc, file);
//   file.close();

//   if (error) {
//     Serial.println("Failed to parse schedules file");
//     numSchedules = 0;
//     return;
//   }

//   numSchedules = 0;
//   JsonArray scheduleArray = doc["schedules"].as<JsonArray>();

//   for (JsonObject schedule : scheduleArray) {
//     if (numSchedules < 20) {
//       schedules[numSchedules].chamberIndex = schedule["chamber"];
//       schedules[numSchedules].hour = schedule["hour"];
//       schedules[numSchedules].minute = schedule["minute"];
//       schedules[numSchedules].taken = false;
//       schedules[numSchedules].alarmActive = false;
//       schedules[numSchedules].callSent = false;
//       schedules[numSchedules].alarmStartTime = 0;
//       numSchedules++;
//     }
//   }

//   Serial.print("Loaded ");
//   Serial.print(numSchedules);
//   Serial.println(" schedules");
// }

// // Save patient phone number to LittleFS
// void savePatientPhone(const String &phone) {
//   File file = LittleFS.open("/phone.txt", "w");
//   if (!file) {
//     Serial.println("Failed to open phone file for writing");
//     return;
//   }

//   file.print(phone);
//   file.close();
//   Serial.println("Patient phone saved to LittleFS");
// }

// // Load patient phone number from LittleFS
// String loadPatientPhone() {
//   if (!LittleFS.exists("/phone.txt")) {
//     Serial.println("No phone file found");
//     return "";
//   }

//   File file = LittleFS.open("/phone.txt", "r");
//   if (!file) {
//     Serial.println("Failed to open phone file");
//     return "";
//   }

//   String phone = file.readString();
//   file.close();

//   Serial.print("Loaded patient phone: ");
//   Serial.println(phone);
//   return phone;
// }

// // Save users to LittleFS
// void saveUsers(User users[], int numUsers) {
//   File file = LittleFS.open("/users.json", "w");
//   if (!file) {
//     Serial.println("Failed to open users file for writing");
//     return;
//   }

//   StaticJsonDocument<2048> doc;
//   JsonArray userArray = doc.createNestedArray("users");

//   for (int i = 0; i < numUsers; i++) {
//     JsonObject user = userArray.createNestedObject();
//     user["username"] = users[i].username;
//     user["password"] = users[i].password;
//     user["role"] = users[i].role;
//     user["phone"] = users[i].phone;
//   }

//   serializeJson(doc, file);
//   file.close();
//   Serial.println("Users saved to LittleFS");
// }

// // Load users from LittleFS
// void loadUsers(User users[], int &numUsers) {
//   if (!LittleFS.exists("/users.json")) {
//     // Create default admin user if file doesn't exist
//     users[0].username = "admin";
//     users[0].password = "admin123";
//     users[0].role = "doctor";
//     users[0].phone = "";
//     numUsers = 1;
//     saveUsers(users, numUsers);
//     return;
//   }

//   File file = LittleFS.open("/users.json", "r");
//   if (!file) {
//     Serial.println("Failed to open users file");
//     numUsers = 0;
//     return;
//   }

//   StaticJsonDocument<2048> doc;
//   DeserializationError error = deserializeJson(doc, file);
//   file.close();

//   if (error) {
//     Serial.println("Failed to parse users file");
//     numUsers = 0;
//     return;
//   }

//   numUsers = 0;
//   JsonArray userArray = doc["users"].as<JsonArray>();

//   for (JsonObject userObj : userArray) {
//     if (numUsers < 10) {
//       users[numUsers].username = userObj["username"].as<String>();
//       users[numUsers].password = userObj["password"].as<String>();
//       users[numUsers].role = userObj["role"].as<String>();
//       users[numUsers].phone = userObj["phone"].as<String>();
//       numUsers++;
//     }
//   }

//   Serial.print("Loaded ");
//   Serial.print(numUsers);
//   Serial.println(" users");
// }

// #endif // FILE_OPERATIONS_H



#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "data_structures.h"

// Function declarations
void saveSchedules(MedicineSchedule schedules[], int numSchedules);
void loadSchedules(MedicineSchedule schedules[], int &numSchedules);
void savePatientPhone(const String &phone);
String loadPatientPhone();
void saveUsers(User users[], int numUsers);
void loadUsers(User users[], int &numUsers);

// Save schedules to LittleFS with error handling
// void saveSchedules(MedicineSchedule schedules[], int numSchedules) {
//   // Make sure LittleFS is mounted
//   if (!LittleFS.begin(false)) {
//     Serial.println("Error: LittleFS not mounted, cannot save schedules");
//     return;
//   }

//   // Create a buffer for the JSON data
//   StaticJsonDocument<2048> doc; // Increase buffer size
//   JsonArray scheduleArray = doc.createNestedArray("schedules");

//   // Add schedules to JSON document
//   for (int i = 0; i < numSchedules; i++) {
//     JsonObject schedule = scheduleArray.createNestedObject();
//     schedule["chamber"] = schedules[i].chamberIndex;
//     schedule["hour"] = schedules[i].hour;
//     schedule["minute"] = schedules[i].minute;
//     // Don't save transient state like 'taken', 'alarmActive', etc.
//   }

//   // Serialize JSON to a String first to verify it's valid
//   String jsonString;
//   serializeJson(doc, jsonString);

//   Serial.print("Saving schedules JSON (length: ");
//   Serial.print(jsonString.length());
//   Serial.println(" bytes)");

//   // Try to open the file for writing
//   File file = LittleFS.open("/schedules.json", "w");
//   if (!file) {
//     Serial.println("Failed to open schedules file for writing");
//     // Try to create parent directories if needed
//     if (LittleFS.mkdir("/")) {
//       Serial.println("Created root directory");
//       file = LittleFS.open("/schedules.json", "w");
//       if (!file) {
//         Serial.println("Still couldn't open file after creating directory");
//         return;
//       }
//     } else {
//       Serial.println("Couldn't create directories");
//       return;
//     }
//   }

//   // Write the JSON data to the file
//   size_t bytesWritten = file.print(jsonString);

//   if (bytesWritten == 0) {
//     Serial.println("Error writing to file");
//   } else {
//     Serial.print("Wrote ");
//     Serial.print(bytesWritten);
//     Serial.println(" bytes to schedules.json");
//   }

//   file.close();
// }


void saveSchedules(MedicineSchedule schedules[], int numSchedules) {
  StaticJsonDocument<2048> doc;
  JsonArray scheduleArray = doc.createNestedArray("schedules");

  for (int i = 0; i < numSchedules; i++) {
    JsonObject schedule = scheduleArray.createNestedObject();
    schedule["chamber"] = schedules[i].chamberIndex;
    schedule["hour"] = schedules[i].hour;
    schedule["minute"] = schedules[i].minute;
  }

  String jsonString;
  serializeJson(doc, jsonString);

  File file = LittleFS.open("/schedules.json", "w");
  if (!file) {
    Serial.println("ERROR: Could not open schedules file for writing");
    return;
  }

  if (file.print(jsonString) == 0) {
    Serial.println("ERROR: Failed to write schedules to file");
  } else {
    Serial.println("SUCCESS: Schedules saved successfully");
  }

  file.close();
}

// Load schedules from LittleFS with error handling
void loadSchedules(MedicineSchedule schedules[], int &numSchedules) {
  // Reset counter
  numSchedules = 0;

  // Make sure LittleFS is mounted
  if (!LittleFS.begin(false)) {
    Serial.println("Error: LittleFS not mounted, cannot load schedules");
    return;
  }

  // Check if file exists
  if (!LittleFS.exists("/schedules.json")) {
    Serial.println("No schedules file found");
    return;
  }

  // Try to open the file for reading
  File file = LittleFS.open("/schedules.json", "r");
  if (!file) {
    Serial.println("Failed to open schedules file for reading");
    return;
  }

  // Get file size
  size_t fileSize = file.size();
  Serial.print("Schedule file size: ");
  Serial.print(fileSize);
  Serial.println(" bytes");

  // Check if file is empty
  if (fileSize == 0) {
    Serial.println("Schedule file is empty");
    file.close();
    return;
  }

  // Allocate a larger buffer for JSON parsing
  StaticJsonDocument<2048> doc;

  // Parse JSON directly from file
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  // Check for parsing errors
  if (error) {
    Serial.print("Failed to parse schedules file: ");
    Serial.println(error.c_str());
    return;
  }

  // Check if the document has a schedules array
  if (!doc.containsKey("schedules")) {
    Serial.println("No schedules array in file");
    return;
  }

  // Extract schedules array
  JsonArray scheduleArray = doc["schedules"].as<JsonArray>();

  // Check if array is empty
  if (scheduleArray.size() == 0) {
    Serial.println("Schedule array is empty");
    return;
  }

  // Extract each schedule
  for (JsonObject schedule : scheduleArray) {
    if (numSchedules < 20) { // Prevent buffer overflow
      // Check if schedule contains all required fields
      if (schedule.containsKey("chamber") &&
          schedule.containsKey("hour") &&
          schedule.containsKey("minute")) {

        schedules[numSchedules].chamberIndex = schedule["chamber"];
        schedules[numSchedules].hour = schedule["hour"];
        schedules[numSchedules].minute = schedule["minute"];
        schedules[numSchedules].taken = false;
        schedules[numSchedules].alarmActive = false;
        schedules[numSchedules].callSent = false;
        schedules[numSchedules].alarmStartTime = 0;
        numSchedules++;
      } else {
        Serial.println("Skipping invalid schedule entry");
      }
    }
  }

  Serial.print("Loaded ");
  Serial.print(numSchedules);
  Serial.println(" schedules");
}

// Save patient phone number with error handling
void savePatientPhone(const String &phone) {
  // Make sure LittleFS is mounted
  if (!LittleFS.begin(false)) {
    Serial.println("Error: LittleFS not mounted, cannot save phone");
    return;
  }

  // Try to open file for writing
  File file = LittleFS.open("/phone.txt", "w");
  if (!file) {
    Serial.println("Failed to open phone file for writing");
    return;
  }

  // Write phone number
  size_t bytesWritten = file.print(phone);

  if (bytesWritten == 0) {
    Serial.println("Error writing phone number to file");
  } else {
    Serial.print("Saved phone number: ");
    Serial.println(phone);
  }

  file.close();
}

// Load patient phone number with error handling
String loadPatientPhone() {
  // Make sure LittleFS is mounted
  if (!LittleFS.begin(false)) {
    Serial.println("Error: LittleFS not mounted, cannot load phone");
    return "";
  }

  // Check if file exists
  if (!LittleFS.exists("/phone.txt")) {
    Serial.println("No phone file found");
    return "";
  }

  // Try to open file for reading
  File file = LittleFS.open("/phone.txt", "r");
  if (!file) {
    Serial.println("Failed to open phone file for reading");
    return "";
  }

  // Read phone number
  String phone = file.readString();
  file.close();

  // Trim whitespace
  phone.trim();

  Serial.print("Loaded patient phone: ");
  Serial.println(phone);
  return phone;
}

// Save users to LittleFS with error handling
void saveUsers(User users[], int numUsers) {
  // Make sure LittleFS is mounted
  if (!LittleFS.begin(false)) {
    Serial.println("Error: LittleFS not mounted, cannot save users");
    return;
  }

  // Create JSON document
  StaticJsonDocument<2048> doc;
  JsonArray userArray = doc.createNestedArray("users");

  // Add users to JSON document
  for (int i = 0; i < numUsers; i++) {
    JsonObject user = userArray.createNestedObject();
    user["username"] = users[i].username;
    user["password"] = users[i].password;
    user["role"] = users[i].role;
    user["phone"] = users[i].phone;
  }

  // Try to open file for writing
  File file = LittleFS.open("/users.json", "w");
  if (!file) {
    Serial.println("Failed to open users file for writing");
    return;
  }

  // Write JSON document to file
  size_t bytesWritten = serializeJson(doc, file);

  if (bytesWritten == 0) {
    Serial.println("Error writing users to file");
  } else {
    Serial.print("Saved ");
    Serial.print(numUsers);
    Serial.println(" users");
  }

  file.close();
}

// Load users from LittleFS with error handling
void loadUsers(User users[], int &numUsers) {
  // Reset counter
  numUsers = 0;

  // Make sure LittleFS is mounted
  if (!LittleFS.begin(false)) {
    Serial.println("Error: LittleFS not mounted, cannot load users");

    // Create default admin user
    users[0].username = "admin";
    users[0].password = "admin";
    users[0].role = "admin";
    users[0].phone = "";
    numUsers = 1;

    // Try to save the default user
    saveUsers(users, numUsers);
    return;
  }

  // Check if file exists
  if (!LittleFS.exists("/users.json")) {
    Serial.println("No users file found");

    // Create default admin user
    users[0].username = "admin";
    users[0].password = "admin";
    users[0].role = "admin";
    users[0].phone = "";
    numUsers = 1;

    // Save the default user
    saveUsers(users, numUsers);
    return;
  }

  // Try to open file for reading
  File file = LittleFS.open("/users.json", "r");
  if (!file) {
    Serial.println("Failed to open users file");

    // Create default admin user
    users[0].username = "admin";
    users[0].password = "admin";
    users[0].role = "admin";
    users[0].phone = "";
    numUsers = 1;

    return;
  }

  // Parse JSON file
  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  // Check for parsing errors
  if (error) {
    Serial.print("Failed to parse users file: ");
    Serial.println(error.c_str());

    // Create default admin user
    users[0].username = "admin";
    users[0].password = "admin";
    users[0].role = "admin";
    users[0].phone = "";
    numUsers = 1;

    return;
  }

  // Check if file has 'users' array
  if (!doc.containsKey("users")) {
    Serial.println("No users array in file");

    // Create default admin user
    users[0].username = "admin";
    users[0].password = "admin";
    users[0].role = "admin";
    users[0].phone = "";
    numUsers = 1;

    return;
  }

  // Extract users from JSON
  JsonArray userArray = doc["users"].as<JsonArray>();

  if (userArray.size() == 0) {
    // Create default admin user if no users found
    users[0].username = "admin";
    users[0].password = "admin";
    users[0].role = "admin";
    users[0].phone = "";
    numUsers = 1;
  } else {
    // Extract existing users
    for (JsonObject userObj : userArray) {
      if (numUsers < 10) {
        users[numUsers].username = userObj["username"].as<String>();
        users[numUsers].password = userObj["password"].as<String>();
        users[numUsers].role = userObj["role"].as<String>();
        users[numUsers].phone = userObj["phone"].as<String>();
        numUsers++;
      }
    }

    // Add default admin if not found
    bool adminFound = false;
    for (int i = 0; i < numUsers; i++) {
      if (users[i].username == "admin" && users[i].role == "admin") {
        adminFound = true;
        break;
      }
    }

    if (!adminFound && numUsers < 10) {
      users[numUsers].username = "admin";
      users[numUsers].password = "admin";
      users[numUsers].role = "admin";
      users[numUsers].phone = "";
      numUsers++;
    }
  }

  Serial.print("Loaded ");
  Serial.print(numUsers);
  Serial.println(" users");
}

#endif // FILE_OPERATIONS_H