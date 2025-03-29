// #ifndef WEB_SERVER_H
// #define WEB_SERVER_H

// #include <Arduino.h>
// #include <ESPAsyncWebServer.h>
// #include <AsyncTCP.h>
// #include <ArduinoJson.h>
// #include "data_structures.h"
// #include "file_operations.h"
// #include "html_assets.h"

// // Function declarations
// void setupWebServer(AsyncWebServer &server, MedicineSchedule schedules[], int &numSchedules, User users[], int &numUsers, String &patientPhone);

// // Set up all web server routes
// void setupWebServer(AsyncWebServer &server, MedicineSchedule schedules[], int &numSchedules, User users[], int &numUsers, String &patientPhone) {
//   // Main route serves the login page
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send(200, "text/html", LOGIN_HTML);
//   });

//   // Authentication endpoints
//   server.on("/login", HTTP_POST, [&users, &numUsers](AsyncWebServerRequest *request){
//     String username = request->getParam("username", true)->value();
//     String password = request->getParam("password", true)->value();
//     String role = request->getParam("role", true)->value();

//     bool authenticated = false;
//     for (int i = 0; i < numUsers; i++) {
//       if (users[i].username == username &&
//           users[i].password == password &&
//           users[i].role == role) {
//         authenticated = true;
//         break;
//       }
//     }

//     if (authenticated) {
//       request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Login successful\"}");
//     } else {
//       request->send(401, "application/json", "{\"status\":\"error\",\"message\":\"Invalid credentials\"}");
//     }
//   });

//   server.on("/signup", HTTP_POST, [&users, &numUsers](AsyncWebServerRequest *request){
//     if (numUsers >= 10) {
//       request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Maximum number of users reached\"}");
//       return;
//     }

//     String username = request->getParam("username", true)->value();
//     String password = request->getParam("password", true)->value();
//     String role = request->getParam("role", true)->value();
//     String phone = request->getParam("phone", true)->value();

//     // Check if username already exists
//     for (int i = 0; i < numUsers; i++) {
//       if (users[i].username == username) {
//         request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Username already exists\"}");
//         return;
//       }
//     }

//     // Add the new user
//     users[numUsers].username = username;
//     users[numUsers].password = password;
//     users[numUsers].role = role;
//     users[numUsers].phone = phone;
//     numUsers++;

//     // Save the updated users list
//     saveUsers(users, numUsers);

//     request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"User registered successfully\"}");
//   });

//   // Medicine schedule endpoints
//   server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send(200, "text/html", DASHBOARD_HTML);
//   });

//   server.on("/schedule", HTTP_POST, [&schedules, &numSchedules](AsyncWebServerRequest *request){
//     if (request->hasParam("schedule", true)) {
//       String scheduleJson = request->getParam("schedule", true)->value();
//       StaticJsonDocument<1024> doc;
//       DeserializationError error = deserializeJson(doc, scheduleJson);

//       if (!error) {
//         numSchedules = 0;
//         JsonArray scheduleArray = doc["schedules"].as<JsonArray>();

//         for (JsonObject schedule : scheduleArray) {
//           if (numSchedules < 20) {
//             schedules[numSchedules].chamberIndex = schedule["chamber"];
//             schedules[numSchedules].hour = schedule["hour"];
//             schedules[numSchedules].minute = schedule["minute"];
//             schedules[numSchedules].taken = false;
//             schedules[numSchedules].alarmActive = false;
//             schedules[numSchedules].callSent = false;
//             schedules[numSchedules].alarmStartTime = 0;
//             numSchedules++;
//           }
//         }

//         saveSchedules(schedules, numSchedules);
//         request->send(200, "application/json", "{\"status\":\"success\"}");
//       } else {
//         request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
//       }
//     } else {
//       request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"No data provided\"}");
//     }
//   });

//   server.on("/getschedule", HTTP_GET, [&schedules, &numSchedules](AsyncWebServerRequest *request){
//     StaticJsonDocument<1024> doc;
//     JsonArray scheduleArray = doc.createNestedArray("schedules");

//     for (int i = 0; i < numSchedules; i++) {
//       JsonObject schedule = scheduleArray.createNestedObject();
//       schedule["chamber"] = schedules[i].chamberIndex;
//       schedule["hour"] = schedules[i].hour;
//       schedule["minute"] = schedules[i].minute;
//       schedule["taken"] = schedules[i].taken;
//     }

//     String jsonString;
//     serializeJson(doc, jsonString);
//     request->send(200, "application/json", jsonString);
//   });

//   server.on("/phone", HTTP_POST, [&patientPhone](AsyncWebServerRequest *request){
//     if (request->hasParam("phone", true)) {
//       patientPhone = request->getParam("phone", true)->value();
//       savePatientPhone(patientPhone);
//       request->send(200, "application/json", "{\"status\":\"success\"}");
//     } else {
//       request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"No phone provided\"}");
//     }
//   });

//   server.on("/getphone", HTTP_GET, [&patientPhone](AsyncWebServerRequest *request){
//     String jsonString = "{\"phone\":\"" + patientPhone + "\"}";
//     request->send(200, "application/json", jsonString);
//   });

//   // Handle not found
//   server.onNotFound([](AsyncWebServerRequest *request){
//     request->send(404, "text/plain", "Not found");
//   });
// }

// #endif // WEB_SERVER_H

// new version

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include "data_structures.h"
#include "file_operations.h"
#include "html_assets.h"

// Function declarations
void setupWebServer(AsyncWebServer &server, MedicineSchedule schedules[], int &numSchedules, User users[], int &numUsers, String &patientPhone);
bool emergency_save_schedule(int chamber, int hour, int minute); // Add this line


// Initialize the default admin user
void initDefaultUser(User users[], int &numUsers)
{
    // Check if users array is empty
    if (numUsers == 0)
    {
        // Create default admin user
        users[0].username = "admin";
        users[0].password = "admin";
        users[0].role = "admin";
        users[0].phone = "";
        numUsers = 1;

        // Save the user to storage
        saveUsers(users, numUsers);

        Serial.println("Created default admin user (username: admin, password: admin)");
    }
}

// Set up all web server routes
void setupWebServer(AsyncWebServer &server, MedicineSchedule schedules[], int &numSchedules, User users[], int &numUsers, String &patientPhone)
{
    // Make sure default user exists
    initDefaultUser(users, numUsers);

    // Main route serves the login page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", LOGIN_HTML); });

    // Authentication endpoint
    server.on("/login", HTTP_POST, [&users, &numUsers](AsyncWebServerRequest *request)
              {
    String username = request->getParam("username", true)->value();
    String password = request->getParam("password", true)->value();
    String role = request->getParam("role", true)->value();

    // Log attempt
    Serial.print("Login attempt: Username=");
    Serial.print(username);
    Serial.print(", Password=");
    Serial.print(password);
    Serial.print(", Role=");
    Serial.println(role);

    // Special case for admin user
    if (username == "admin" && password == "admin") {
      request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Login successful\"}");
      return;
    }

    // Check other users
    bool authenticated = false;
    for (int i = 0; i < numUsers; i++) {
      if (users[i].username == username &&
          users[i].password == password) {
        authenticated = true;
        break;
      }
    }

    if (authenticated) {
      request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Login successful\"}");
    } else {
      request->send(401, "application/json", "{\"status\":\"error\",\"message\":\"Invalid credentials\"}");
    } });

    // Medicine schedule endpoints
    server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", DASHBOARD_HTML); });

    //   server.on("/schedule", HTTP_POST, [&schedules, &numSchedules](AsyncWebServerRequest *request){
    //     if (request->hasParam("schedule", true)) {
    //       String scheduleJson = request->getParam("schedule", true)->value();
    //       StaticJsonDocument<1024> doc;
    //       DeserializationError error = deserializeJson(doc, scheduleJson);

    //       if (!error) {
    //         numSchedules = 0;
    //         JsonArray scheduleArray = doc["schedules"].as<JsonArray>();

    //         for (JsonObject schedule : scheduleArray) {
    //           if (numSchedules < 20) {
    //             schedules[numSchedules].chamberIndex = schedule["chamber"];
    //             schedules[numSchedules].hour = schedule["hour"];
    //             schedules[numSchedules].minute = schedule["minute"];
    //             schedules[numSchedules].taken = false;
    //             schedules[numSchedules].alarmActive = false;
    //             schedules[numSchedules].callSent = false;
    //             schedules[numSchedules].alarmStartTime = 0;
    //             numSchedules++;
    //           }
    //         }

    //         saveSchedules(schedules, numSchedules);
    //         request->send(200, "application/json", "{\"status\":\"success\"}");
    //       } else {
    //         request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
    //       }
    //     } else {
    //       request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"No data provided\"}");
    //     }
    //   });

    // Replace this section in your web_server.h file

    server.on("/schedule", HTTP_POST, [&schedules, &numSchedules](AsyncWebServerRequest *request)
              {
    if (request->hasParam("schedule", true)) {
      String scheduleJson = request->getParam("schedule", true)->value();
      Serial.print("Received schedule JSON: ");
      Serial.println(scheduleJson);

      // Create a larger buffer for JSON parsing
      StaticJsonDocument<2048> doc;
      DeserializationError error = deserializeJson(doc, scheduleJson);

      if (!error) {
        // Clear existing schedules
        numSchedules = 0;

        // Check if JSON contains schedules array
        if (doc.containsKey("schedules") && doc["schedules"].is<JsonArray>()) {
          JsonArray scheduleArray = doc["schedules"].as<JsonArray>();

          // Process each schedule
          for (JsonObject schedule : scheduleArray) {
            if (numSchedules < 20) { // Prevent buffer overflow
              // Check if schedule contains all required fields
              if (schedule.containsKey("chamber") &&
                  schedule.containsKey("hour") &&
                  schedule.containsKey("minute")) {

                schedules[numSchedules].chamberIndex = schedule["chamber"].as<int>();
                schedules[numSchedules].hour = schedule["hour"].as<int>();
                schedules[numSchedules].minute = schedule["minute"].as<int>();
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

          // Save the schedules to storage
          saveSchedules(schedules, numSchedules);

          Serial.print("Updated schedules. New count: ");
          Serial.println(numSchedules);

          request->send(200, "application/json", "{\"status\":\"success\",\"count\":" + String(numSchedules) + "}");
        } else {
          Serial.println("JSON does not contain valid schedules array");
          request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON structure\"}");
        }
      } else {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
        request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON format\"}");
      }
    } else {
      Serial.println("No schedule parameter provided");
      request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"No data provided\"}");
    } });

    server.on("/getschedule", HTTP_GET, [&schedules, &numSchedules](AsyncWebServerRequest *request)
              {
    StaticJsonDocument<1024> doc;
    JsonArray scheduleArray = doc.createNestedArray("schedules");

    for (int i = 0; i < numSchedules; i++) {
      JsonObject schedule = scheduleArray.createNestedObject();
      schedule["chamber"] = schedules[i].chamberIndex;
      schedule["hour"] = schedules[i].hour;
      schedule["minute"] = schedules[i].minute;
      schedule["taken"] = schedules[i].taken;
    }

    String jsonString;
    serializeJson(doc, jsonString);
    request->send(200, "application/json", jsonString); });

    server.on("/phone", HTTP_POST, [&patientPhone](AsyncWebServerRequest *request)
              {
    if (request->hasParam("phone", true)) {
      patientPhone = request->getParam("phone", true)->value();
      savePatientPhone(patientPhone);
      request->send(200, "application/json", "{\"status\":\"success\"}");
    } else {
      request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"No phone provided\"}");
    } });

    server.on("/getphone", HTTP_GET, [&patientPhone](AsyncWebServerRequest *request)
              {
    String jsonString = "{\"phone\":\"" + patientPhone + "\"}";
    request->send(200, "application/json", jsonString); });

    // Add test endpoint for emergency schedule saving
    server.on("/test_save", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    bool success = emergency_save_schedule(1, 8, 30);

    if (success) {
      request->send(200, "text/plain", "Schedule saved successfully!");
    } else {
      request->send(500, "text/plain", "Failed to save schedule!");
    } });

    // Handle not found
    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(404, "text/plain", "Not found"); });
}

#endif // WEB_SERVER_H