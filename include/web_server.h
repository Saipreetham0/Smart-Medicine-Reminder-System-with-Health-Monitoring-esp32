

// #ifndef WEB_SERVER_H
// #define WEB_SERVER_H

// #include <Arduino.h>
// #include <ESPAsyncWebServer.h>
// #include <AsyncTCP.h>
// #include <ArduinoJson.h>
// #include "file_storage.h"
// #include "html_assets.h"

// // Set up all web server routes
// void setupWebServer(AsyncWebServer &server) {
//   // Initialize filesystem and load saved data
//   initFileSystem();
//   loadAllData();

//   // Main route serves the login page
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send(200, "text/html", LOGIN_HTML);
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

//       Serial.print("Received schedule request - Chamber: ");
//       Serial.print(chamber);
//       Serial.print(", Hour: ");
//       Serial.print(hour);
//       Serial.print(", Minute: ");
//       Serial.println(minute);

//       if (addSchedule(chamber, hour, minute)) {
//         printAllSchedules(); // Debug print
//         request->send(200, "application/json", "{\"status\":\"success\"}");
//       } else {
//         request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to add schedule\"}");
//       }
//     } else {
//       Serial.println("Missing parameters in add_schedule request");
//       request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing parameters\"}");
//     }
//   });

//   // Get schedules endpoint
//   server.on("/get_schedules", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String json = getSchedulesJson();
//     Serial.print("Sending schedules JSON: ");
//     Serial.println(json);
//     request->send(200, "application/json", json);
//   });

//   // Remove schedule endpoint
//   server.on("/remove_schedule", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("index", true)) {
//       int index = request->getParam("index", true)->value().toInt();

//       Serial.print("Request to remove schedule at index: ");
//       Serial.println(index);

//       if (removeSchedule(index)) {
//         printAllSchedules(); // Debug print
//         request->send(200, "application/json", "{\"status\":\"success\"}");
//       } else {
//         request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to remove schedule\"}");
//       }
//     } else {
//       Serial.println("Missing index parameter in remove_schedule request");
//       request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing index parameter\"}");
//     }
//   });

//   // Set phone endpoint
//   server.on("/set_phone", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("phone", true)) {
//       String phone = request->getParam("phone", true)->value();

//       Serial.print("Setting phone number to: ");
//       Serial.println(phone);

//       if (setPatientPhone(phone)) {
//         printAllSchedules(); // Debug print
//         request->send(200, "application/json", "{\"status\":\"success\"}");
//       } else {
//         request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to save phone number\"}");
//       }
//     } else {
//       Serial.println("Missing phone parameter in set_phone request");
//       request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing phone parameter\"}");
//     }
//   });

//   // Get phone endpoint
//   server.on("/get_phone", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String json = "{\"phone\":\"" + g_patientPhone + "\"}";
//     Serial.print("Sending phone JSON: ");
//     Serial.println(json);
//     request->send(200, "application/json", json);
//   });

//   // Clear all schedules endpoint
//   server.on("/clear_schedules", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (clearSchedules()) {
//       printAllSchedules(); // Debug print
//       request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"All schedules cleared\"}");
//     } else {
//       request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to clear schedules\"}");
//     }
//   });

//   // Format filesystem endpoint (for emergency recovery)
//   server.on("/format_fs", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (LittleFS.format()) {
//       initFileSystem();
//       loadAllData();
//       request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Filesystem formatted\"}");
//     } else {
//       request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to format filesystem\"}");
//     }
//   });

//   // Debug endpoint to print all schedules
//   server.on("/debug", HTTP_GET, [](AsyncWebServerRequest *request) {
//     printAllSchedules();
//     String response = "Current schedules: " + String(g_numSchedules) +
//                      ", Phone: " + g_patientPhone +
//                      ", See serial monitor for details";
//     request->send(200, "text/plain", response);
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
#include <RTClib.h>
#include "file_storage.h"
#include "html_assets.h"

// External declarations
extern RTC_DS3231 rtc;
extern String getCurrentTimeString();
extern String getCurrentDateString();
extern String getTimeAndDateJson();

// Set up all web server routes
void setupWebServer(AsyncWebServer &server) {
  // Initialize filesystem and load saved data
  initFileSystem();
  loadAllData();

  // Main route serves the login page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", LOGIN_HTML);
  });

  // Dashboard endpoint
  server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", DASHBOARD_HTML);
  });

  // Get current time endpoint
  server.on("/get_time", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = getTimeAndDateJson();
    request->send(200, "application/json", json);
  });

  // Add schedule endpoint
  server.on("/add_schedule", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("chamber", true) &&
        request->hasParam("hour", true) &&
        request->hasParam("minute", true)) {

      int chamber = request->getParam("chamber", true)->value().toInt();
      int hour = request->getParam("hour", true)->value().toInt();
      int minute = request->getParam("minute", true)->value().toInt();

      Serial.print("[");
      Serial.print(getCurrentTimeString());
      Serial.print("] Received schedule request - Chamber: ");
      Serial.print(chamber);
      Serial.print(", Time: ");
      Serial.print(hour);
      Serial.print(":");
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
    Serial.print("[");
    Serial.print(getCurrentTimeString());
    Serial.print("] Sending schedules JSON: ");
    Serial.println(json);
    request->send(200, "application/json", json);
  });

  // Remove schedule endpoint
  server.on("/remove_schedule", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("index", true)) {
      int index = request->getParam("index", true)->value().toInt();

      Serial.print("[");
      Serial.print(getCurrentTimeString());
      Serial.print("] Request to remove schedule at index: ");
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

      Serial.print("[");
      Serial.print(getCurrentTimeString());
      Serial.print("] Setting phone number to: ");
      Serial.println(phone);

      if (setPatientPhone(phone)) {
        printAllSchedules(); // Debug print
        request->send(200, "application/json", "{\"status\":\"success\"}");
      } else {
        request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to save phone number\"}");
      }
    } else {
      Serial.println("Missing phone parameter in set_phone request");
      request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing phone parameter\"}");
    }
  });

  // Get phone endpoint
  server.on("/get_phone", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{\"phone\":\"" + g_patientPhone + "\"}";
    Serial.print("[");
    Serial.print(getCurrentTimeString());
    Serial.print("] Sending phone JSON: ");
    Serial.println(json);
    request->send(200, "application/json", json);
  });

  // Clear all schedules endpoint
  server.on("/clear_schedules", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (clearSchedules()) {
      printAllSchedules(); // Debug print
      request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"All schedules cleared\"}");
    } else {
      request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to clear schedules\"}");
    }
  });

  // Format filesystem endpoint (for emergency recovery)
  server.on("/format_fs", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (LittleFS.format()) {
      initFileSystem();
      loadAllData();
      request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Filesystem formatted\"}");
    } else {
      request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to format filesystem\"}");
    }
  });

  // Set RTC time endpoint
  server.on("/set_time", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("year", true) &&
        request->hasParam("month", true) &&
        request->hasParam("day", true) &&
        request->hasParam("hour", true) &&
        request->hasParam("minute", true) &&
        request->hasParam("second", true)) {

      int year = request->getParam("year", true)->value().toInt();
      int month = request->getParam("month", true)->value().toInt();
      int day = request->getParam("day", true)->value().toInt();
      int hour = request->getParam("hour", true)->value().toInt();
      int minute = request->getParam("minute", true)->value().toInt();
      int second = request->getParam("second", true)->value().toInt();

      // Create a DateTime object
      DateTime newTime(year, month, day, hour, minute, second);

      // Set the RTC time
      rtc.adjust(newTime);

      Serial.print("[");
      Serial.print(getCurrentTimeString());
      Serial.println("] RTC time updated");

      request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Time updated\"}");
    } else {
      Serial.println("Missing time parameters");
      request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing time parameters\"}");
    }
  });

  // Debug endpoint to print all schedules
  server.on("/debug", HTTP_GET, [](AsyncWebServerRequest *request) {
    printAllSchedules();
    String response = "Current time: " + getCurrentTimeString() +
                     ", Date: " + getCurrentDateString() +
                     ", Schedules: " + String(g_numSchedules) +
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