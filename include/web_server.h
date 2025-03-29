

// //new v 1.2 version


// #ifndef WEB_SERVER_H
// #define WEB_SERVER_H

// #include <Arduino.h>
// #include <ESPAsyncWebServer.h>
// #include <AsyncTCP.h>
// #include <ArduinoJson.h>
// #include "memory_storage.h"
// #include "html_assets.h"

// // Set up all web server routes
// void setupWebServer(AsyncWebServer &server) {
//   // Make sure default user exists
//   initDefaultUser();

//   // Main route serves the login page
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send(200, "text/html", LOGIN_HTML);
//   });

//   // Authentication endpoint - just accept all logins
//   server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request) {
//     request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Login successful\"}");
//   });

//   // Dashboard endpoint
//   server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send(200, "text/html", DASHBOARD_HTML);
//   });

//   // Add schedule endpoint
//   server.on("/add_schedule", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("chamber", true) &&
//         request->hasParam("hour", true) &&
//         request->hasParam("minute", true)) {

//       int chamber = request->getParam("chamber", true)->value().toInt();
//       int hour = request->getParam("hour", true)->value().toInt();
//       int minute = request->getParam("minute", true)->value().toInt();

//       if (addSchedule(chamber, hour, minute)) {
//         request->send(200, "application/json", "{\"status\":\"success\"}");
//       } else {
//         request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to add schedule\"}");
//       }
//     } else {
//       request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing parameters\"}");
//     }
//   });

//   // Get schedules endpoint
//   server.on("/get_schedules", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send(200, "application/json", getSchedulesJson());
//   });

//   // Remove schedule endpoint
//   server.on("/remove_schedule", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("index", true)) {
//       int index = request->getParam("index", true)->value().toInt();

//       if (removeSchedule(index)) {
//         request->send(200, "application/json", "{\"status\":\"success\"}");
//       } else {
//         request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to remove schedule\"}");
//       }
//     } else {
//       request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing index parameter\"}");
//     }
//   });

//   // Set phone endpoint
//   server.on("/set_phone", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("phone", true)) {
//       String phone = request->getParam("phone", true)->value();
//       setPatientPhone(phone);
//       request->send(200, "application/json", "{\"status\":\"success\"}");
//     } else {
//       request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing phone parameter\"}");
//     }
//   });

//   // Get phone endpoint
//   server.on("/get_phone", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String json = "{\"phone\":\"" + g_patientPhone + "\"}";
//     request->send(200, "application/json", json);
//   });

//   // Handle not found
//   server.onNotFound([](AsyncWebServerRequest *request) {
//     request->send(404, "text/plain", "Not found");
//   });
// }

// #endif // WEB_SERVER_H

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include "memory_storage.h"
#include "html_assets.h"

// Set up all web server routes
void setupWebServer(AsyncWebServer &server) {
  // Initialize storage
  initStorage();

  // Main route serves the login page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", LOGIN_HTML);
  });

  // Dashboard endpoint
  server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", DASHBOARD_HTML);
  });

  // Add schedule endpoint
  server.on("/add_schedule", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("chamber", true) &&
        request->hasParam("hour", true) &&
        request->hasParam("minute", true)) {

      int chamber = request->getParam("chamber", true)->value().toInt();
      int hour = request->getParam("hour", true)->value().toInt();
      int minute = request->getParam("minute", true)->value().toInt();

      Serial.print("Received schedule request - Chamber: ");
      Serial.print(chamber);
      Serial.print(", Hour: ");
      Serial.print(hour);
      Serial.print(", Minute: ");
      Serial.println(minute);

      if (addSchedule(chamber, hour, minute)) {
        printAllSchedules(); // Debug print
        request->send(200, "application/json", "{\"status\":\"success\"}");
      } else {
        request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to add schedule\"}");
      }
    } else {
      Serial.println("Missing parameters in add_schedule request");
      request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing parameters\"}");
    }
  });

  // Get schedules endpoint
  server.on("/get_schedules", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = getSchedulesJson();
    Serial.print("Sending schedules JSON: ");
    Serial.println(json);
    request->send(200, "application/json", json);
  });

  // Remove schedule endpoint
  server.on("/remove_schedule", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("index", true)) {
      int index = request->getParam("index", true)->value().toInt();

      Serial.print("Request to remove schedule at index: ");
      Serial.println(index);

      if (removeSchedule(index)) {
        printAllSchedules(); // Debug print
        request->send(200, "application/json", "{\"status\":\"success\"}");
      } else {
        request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to remove schedule\"}");
      }
    } else {
      Serial.println("Missing index parameter in remove_schedule request");
      request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing index parameter\"}");
    }
  });

  // Set phone endpoint
  server.on("/set_phone", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("phone", true)) {
      String phone = request->getParam("phone", true)->value();

      Serial.print("Setting phone number to: ");
      Serial.println(phone);

      setPatientPhone(phone);
      printAllSchedules(); // Debug print
      request->send(200, "application/json", "{\"status\":\"success\"}");
    } else {
      Serial.println("Missing phone parameter in set_phone request");
      request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing phone parameter\"}");
    }
  });

  // Get phone endpoint
  server.on("/get_phone", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{\"phone\":\"" + g_patientPhone + "\"}";
    Serial.print("Sending phone JSON: ");
    Serial.println(json);
    request->send(200, "application/json", json);
  });

  // Debug endpoint to print all schedules
  server.on("/debug", HTTP_GET, [](AsyncWebServerRequest *request) {
    printAllSchedules();
    String response = "Current schedules: " + String(g_numSchedules) +
                     ", Phone: " + g_patientPhone +
                     ", See serial monitor for details";
    request->send(200, "text/plain", response);
  });

  // Handle not found
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });
}

#endif // WEB_SERVER_H