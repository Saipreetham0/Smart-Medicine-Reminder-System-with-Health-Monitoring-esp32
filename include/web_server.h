

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include "file_storage.h"
#include "html_assets.h"
#include "gsm_module.h" // Make sure this is included

// External declarations
extern RTC_DS3231 rtc;
extern String getCurrentTimeString();
extern String getCurrentDateString();
extern String getTimeAndDateJson();
extern GSMModule gsm; // Add this line

// Set up all web server routes
void setupWebServer(AsyncWebServer &server)
{
  // Initialize filesystem and load saved data
  initFileSystem();
  loadAllData();

  // Main route serves the login page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", LOGIN_HTML); });

  // Dashboard endpoint
  server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", DASHBOARD_HTML); });

  // Get current time endpoint
  server.on("/get_time", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getTimeAndDateJson();
    request->send(200, "application/json", json); });

  // Add schedule endpoint
  server.on("/add_schedule", HTTP_POST, [](AsyncWebServerRequest *request)
            {
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
    } });

  // Get schedules endpoint
  server.on("/get_schedules", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getSchedulesJson();
    Serial.print("[");
    Serial.print(getCurrentTimeString());
    Serial.print("] Sending schedules JSON: ");
    Serial.println(json);
    request->send(200, "application/json", json); });

  // Remove schedule endpoint
  server.on("/remove_schedule", HTTP_POST, [](AsyncWebServerRequest *request)
            {
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
    } });

  // Set phone endpoint
  server.on("/set_phone", HTTP_POST, [](AsyncWebServerRequest *request)
            {
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
    } });

  // Get phone endpoint
  server.on("/get_phone", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = "{\"phone\":\"" + g_patientPhone + "\"}";
    Serial.print("[");
    Serial.print(getCurrentTimeString());
    Serial.print("] Sending phone JSON: ");
    Serial.println(json);
    request->send(200, "application/json", json); });

  // Clear all schedules endpoint
  server.on("/clear_schedules", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    if (clearSchedules()) {
      printAllSchedules(); // Debug print
      request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"All schedules cleared\"}");
    } else {
      request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to clear schedules\"}");
    } });

  // Format filesystem endpoint (for emergency recovery)
  server.on("/format_fs", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    if (LittleFS.format()) {
      initFileSystem();
      loadAllData();
      request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Filesystem formatted\"}");
    } else {
      request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to format filesystem\"}");
    } });

  // Set RTC time endpoint
  server.on("/set_time", HTTP_POST, [](AsyncWebServerRequest *request)
            {
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
    } });

  // Debug endpoint to print all schedules
  server.on("/debug", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    printAllSchedules();
    String response = "Current time: " + getCurrentTimeString() +
                     ", Date: " + getCurrentDateString() +
                     ", Schedules: " + String(g_numSchedules) +
                     ", Phone: " + g_patientPhone +
                     ", See serial monitor for details";
    request->send(200, "text/plain", response); });

  // Handle not found
  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/plain", "Not found"); });

  // GSM Test endpoint
  // server.on("/gsm_test", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send(200, "text/html", R"rawliteral(
  //     <!DOCTYPE html>
  //     <html lang="en">
  //     <head>
  //       <meta charset="UTF-8">
  //       <meta name="viewport" content="width=device-width, initial-scale=1.0">
  //       <title>GSM Module Test</title>

  server.on("/gsm_test", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  // Prepare GSM status info
  // String gsmStatus = gsm.isInitialized() ?
  //   "<div style='background-color: #d4edda; color: #155724; padding: 10px; border-radius: 4px; margin-bottom: 20px;'>GSM Module Status: Initialized ✓</div>" :
  //   "<div style='background-color: #f8d7da; color: #721c24; padding: 10px; border-radius: 4px; margin-bottom: 20px;'>GSM Module Status: NOT INITIALIZED ✗</div>";

  request->send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>GSM Module Test</title>
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: #f5f5f5;
          margin: 0;
          padding: 20px;
        }
        .container {
          max-width: 800px;
          margin: 0 auto;
          background-color: white;
          border-radius: 8px;
          box-shadow: 0 2px 6px rgba(0,0,0,0.1);
          padding: 20px;
        }
        h1 {
          color: #1a73e8;
        }
        .card {
          background-color: white;
          border: 1px solid #e0e0e0;
          border-radius: 8px;
          padding: 20px;
          margin-bottom: 20px;
        }
        h2 {
          color: #202124;
          margin-top: 0;
        }
        .form-group {
          margin-bottom: 15px;
        }
        label {
          display: block;
          margin-bottom: 5px;
          font-weight: 500;
        }
        input, textarea {
          width: 100%;
          padding: 8px;
          border: 1px solid #dadce0;
          border-radius: 4px;
          box-sizing: border-box;
        }
        textarea {
          height: 100px;
          resize: vertical;
        }
        .button {
          background-color: #1a73e8;
          color: white;
          border: none;
          border-radius: 4px;
          padding: 10px 15px;
          cursor: pointer;
          font-weight: 500;
          margin-right: 10px;
        }
        .back-button {
          background-color: #f1f3f4;
          color: #5f6368;
          border: none;
          border-radius: 4px;
          padding: 8px 15px;
          cursor: pointer;
          text-decoration: none;
          display: inline-block;
          margin-top: 20px;
        }
        .status {
          padding: 10px;
          margin-top: 10px;
          border-radius: 4px;
          display: none;
        }
        .success {
          background-color: #d4edda;
          color: #155724;
          display: block;
        }
        .error {
          background-color: #f8d7da;
          color: #721c24;
          display: block;
        }
      </style>
    </head>
    <body>
      <div class="container">
        <h1>GSM Module Test</h1>


                )rawliteral"
                //  +
                //  gsmStatus
                //  +
                  R"rawliteral(


        <div class="card">
          <h2>Send SMS</h2>
          <div class="form-group">
            <label for="sms-phone">Phone Number</label>
            <input type="tel" id="sms-phone" placeholder="Enter phone number (with country code)">
          </div>
          <div class="form-group">
            <label for="sms-message">Message</label>
            <textarea id="sms-message" placeholder="Enter your message"></textarea>
          </div>
          <button class="button" id="send-sms-button">Send SMS</button>
          <div id="sms-status" class="status"></div>
        </div>

        <div class="card">
          <h2>Make Call</h2>
          <div class="form-group">
            <label for="call-phone">Phone Number</label>
            <input type="tel" id="call-phone" placeholder="Enter phone number (with country code)">
          </div>
          <button class="button" id="make-call-button">Make Call</button>
          <div id="call-status" class="status"></div>
        </div>

        <div class="card">
          <h2>AT Command Test</h2>
          <div class="form-group">
            <label for="at-command">AT Command</label>
            <input type="text" id="at-command" placeholder="Enter AT command (e.g. AT+CSQ)">
          </div>
          <button class="button" id="send-at-button">Send Command</button>
          <div id="at-status" class="status"></div>
        </div>


        <div class="card">
  <h2>GSM Module Control</h2>
  <button class="button" id="reinit-gsm-button">Reinitialize GSM Module</button>
  <div id="reinit-status" class="status"></div>
</div>

        <a href="/" class="back-button">Back to Home</a>
      </div>

      <script>
        // Function to show status messages
        function showStatus(elementId, message, isSuccess) {
          const statusElement = document.getElementById(elementId);
          statusElement.textContent = message;
          statusElement.className = 'status ' + (isSuccess ? 'success' : 'error');

          // Hide status message after 5 seconds
          setTimeout(() => {
            statusElement.style.display = 'none';
          }, 5000);
        }

        // Send SMS
        document.getElementById('send-sms-button').addEventListener('click', function() {
          const phone = document.getElementById('sms-phone').value;
          const message = document.getElementById('sms-message').value;

          if (!phone || !message) {
            showStatus('sms-status', 'Please enter both phone number and message', false);
            return;
          }

          fetch('/send_sms', {
            method: 'POST',
            headers: {
              'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: `phone=${encodeURIComponent(phone)}&message=${encodeURIComponent(message)}`
          })
          .then(response => response.json())
          .then(data => {
            if (data.status === 'success') {
              showStatus('sms-status', 'SMS sent successfully!', true);
            } else {
              showStatus('sms-status', data.message || 'Failed to send SMS', false);
            }
          })
          .catch(error => {
            console.error('Error:', error);
            showStatus('sms-status', 'An error occurred', false);
          });
        });

        // Make Call
        document.getElementById('make-call-button').addEventListener('click', function() {
          const phone = document.getElementById('call-phone').value;

          if (!phone) {
            showStatus('call-status', 'Please enter a phone number', false);
            return;
          }

          fetch('/make_call', {
            method: 'POST',
            headers: {
              'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: `phone=${encodeURIComponent(phone)}`
          })
          .then(response => response.json())
          .then(data => {
            if (data.status === 'success') {
              showStatus('call-status', 'Call initiated!', true);
            } else {
              showStatus('call-status', data.message || 'Failed to make call', false);
            }
          })
          .catch(error => {
            console.error('Error:', error);
            showStatus('call-status', 'An error occurred', false);
          });
        });

        // Send AT Command
        document.getElementById('send-at-button').addEventListener('click', function() {
          const command = document.getElementById('at-command').value;

          if (!command) {
            showStatus('at-status', 'Please enter an AT command', false);
            return;
          }

          fetch('/send_at', {
            method: 'POST',
            headers: {
              'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: `command=${encodeURIComponent(command)}`
          })
          .then(response => response.json())
          .then(data => {
            if (data.status === 'success') {
              showStatus('at-status', `Command sent! Response: ${data.response}`, true);
            } else {
              showStatus('at-status', data.message || 'Failed to send command', false);
            }
          })
          .catch(error => {
            console.error('Error:', error);
            showStatus('at-status', 'An error occurred', false);
          });
        });

        document.getElementById('reinit-gsm-button').addEventListener('click', function() {
  fetch('/reinit_gsm', {
    method: 'POST'
  })
  .then(response => response.json())
  .then(data => {
    if (data.status === 'success') {
      showStatus('reinit-status', 'GSM Module reinitialized successfully! Refreshing page...', true);
      setTimeout(() => {
        window.location.reload();
      }, 2000);
    } else {
      showStatus('reinit-status', data.message || 'Failed to reinitialize GSM module', false);
    }
  })
  .catch(error => {
    console.error('Error:', error);
    showStatus('reinit-status', 'An error occurred', false);
  });
});
      </script>
    </body>
    </html>
  )rawliteral"); });

  // GSM API Endpoints
  server.on("/send_sms", HTTP_POST, [](AsyncWebServerRequest *request)
            {
  String phone = "";
  String message = "";

  if (request->hasParam("phone", true) && request->hasParam("message", true)) {
    phone = request->getParam("phone", true)->value();
    message = request->getParam("message", true)->value();

    Serial.print("[Web] Sending SMS to: ");
    Serial.println(phone);
    Serial.print("Message: ");
    Serial.println(message);

    if (gsm.sendSMS(phone, message)) {
      request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"SMS sent successfully\"}");
    } else {
      request->send(200, "application/json", "{\"status\":\"error\",\"message\":\"Failed to send SMS\"}");
    }
  } else {
    request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing parameters\"}");
  } });

  server.on("/make_call", HTTP_POST, [](AsyncWebServerRequest *request)
            {
  String phone = "";

  if (request->hasParam("phone", true)) {
    phone = request->getParam("phone", true)->value();

    Serial.print("[Web] Making call to: ");
    Serial.println(phone);

    if (gsm.makeCall(phone)) {
      request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Call initiated\"}");
    } else {
      request->send(200, "application/json", "{\"status\":\"error\",\"message\":\"Failed to make call\"}");
    }
  } else {
    request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing phone parameter\"}");
  } });

  // This endpoint requires an update to the GSM module to add a sendATCommand method
  // server.on("/send_at", HTTP_POST, [](AsyncWebServerRequest *request)
  //           {
  // String command = "";

  // if (request->hasParam("command", true)) {
  //   command = request->getParam("command", true)->value();

  //   Serial.print("[Web] Sending AT command: ");
  //   Serial.println(command);

  //   // String response = gsm.sendATCommandWithResponse(command);
  //   String response = gsm.sendATCommand(command);  // Changed method name here


  //   // Escape quotes in the response to make valid JSON
  //   response.replace("\"", "\\\"");
  //   response.replace("\r", "\\r");
  //   response.replace("\n", "\\n");

  //   request->send(200, "application/json", "{\"status\":\"success\",\"response\":\"" + response + "\"}");
  // } else {
  //   request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing command parameter\"}");
  // } });

  // server.on("/reinit_gsm", HTTP_POST, [](AsyncWebServerRequest *request)
  //           {
  //   Serial.println("[Web] Reinitializing GSM module...");

  //   if (gsm.begin(9600)) {
  //     Serial.println("[Web] GSM module reinitialized successfully!");
  //     request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"GSM module reinitialized successfully\"}");
  //   } else {
  //     Serial.println("[Web] Failed to reinitialize GSM module");
  //     request->send(200, "application/json", "{\"status\":\"error\",\"message\":\"Failed to reinitialize GSM module\"}");
  //   } });
}

#endif // WEB_SERVER_H