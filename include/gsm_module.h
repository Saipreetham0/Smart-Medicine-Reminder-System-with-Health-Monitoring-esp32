


// #ifndef GSM_MODULE_H
// #define GSM_MODULE_H

// #include <Arduino.h>

// class GSMModule {
// private:
//     HardwareSerial& gsmSerial;
//     bool initialized = false;
//     bool incomingCall = false;
//     bool outgoingCallDetected = false;
//     String newSMSIndex;
//     unsigned long lastCommandTime = 0;
//     const unsigned long commandCooldown = 1000; // 1 second cooldown between commands

// public:
//     // Constructor
//     GSMModule(HardwareSerial& serial) : gsmSerial(serial) {}

//     // Initialize GSM module with specified baud rate (default 9600)
//     bool begin(unsigned long baudRate = 9600) {
//         gsmSerial.begin(baudRate, SERIAL_8N1, 16, 17); // RX=16, TX=17 for ESP32
//         delay(3000); // Give module time to startup

//         Serial.println("[GSM] Initializing SIM800L module...");

//         // Basic AT command test
//         if (!ATCommandOK("AT", true)) {
//             Serial.println("[GSM] Error: Module not responding to AT commands");
//             return false;
//         }

//         // Turn off echo
//         ATCommandOK("ATE0", true);

//         // Set SMS to text mode
//         if (!ATCommandOK("AT+CMGF=1", true)) {
//             Serial.println("[GSM] Error: Failed to set SMS text mode");
//             return false;
//         }

//         // Set new message indications
//         ATCommandOK("AT+CNMI=1,2,0,0,0", true);

//         // Check if module is registered to network
//         if (!ATCommandOK("AT+CREG?", true)) {
//             Serial.println("[GSM] Warning: Module not registered to network");
//             // Continue anyway as it might register later
//         }

//         // Check signal quality
//         ATCommandOK("AT+CSQ", true);

//         initialized = true;
//         Serial.println("[GSM] Module initialized successfully");
//         return true;
//     }

//     // Make a phone call
//     bool makeCall(const String &phoneNumber) {
//         if (!checkInitialized() || !checkCooldown()) return false;

//         Serial.print("[GSM] Making call to: ");
//         Serial.println(phoneNumber);

//         // Reset flags
//         incomingCall = false;
//         outgoingCallDetected = false;

//         // Set audio settings
//         ATCommandOK("AT+CLVL=5", true); // Set volume level

//         // Command to make a call (semicolon is important for voice call)
//         if (!ATCommandOK("ATD" + phoneNumber + ";", true)) {
//             Serial.println("[GSM] Failed to initiate call");
//             return false;
//         }

//         // Let the call ring for some time (20 seconds)
//         delay(20000);

//         // Hang up the call
//         ATCommandOK("ATH", true);
//         Serial.println("[GSM] Call ended");
//         return true;
//     }

//     // Send an SMS
//     bool sendSMS(const String &phoneNumber, const String &message) {
//         if (!checkInitialized() || !checkCooldown()) return false;

//         Serial.print("[GSM] Sending SMS to: ");
//         Serial.println(phoneNumber);

//         // Set SMS text mode
//         if (!ATCommandOK("AT+CMGF=1", true)) {
//             Serial.println("[GSM] Failed to set SMS text mode");
//             return false;
//         }

//         // Set recipient
//         if (!sendBlockingATCommand("AT+CMGS=\"" + phoneNumber + "\"", true)) {
//             Serial.println("[GSM] Failed to set recipient number");
//             return false;
//         }

//         // Send message content
//         gsmSerial.print(message);
//         delay(500);

//         // Send Ctrl+Z to finalize
//         gsmSerial.write(26);

//         // Wait for message to send
//         delay(5000);

//         Serial.println("[GSM] SMS sent successfully");
//         return true;
//     }

//     // Hang up any active call
//     bool hangUp() {
//         if (!checkInitialized() || !checkCooldown()) return false;

//         Serial.println("[GSM] Hanging up call");
//         return ATCommandOK("ATH", true);
//     }

//     // Get signal quality (0-31, 99=unknown)
//     int getSignalQuality() {
//         if (!checkInitialized() || !checkCooldown()) return -1;

//         gsmSerial.println("AT+CSQ");
//         delay(1000);

//         String response = "";
//         unsigned long timeout = millis() + 5000;

//         while (millis() < timeout) {
//             if (gsmSerial.available()) {
//                 char c = gsmSerial.read();
//                 response += c;

//                 // Look for signal quality response
//                 if (response.indexOf("+CSQ:") != -1) {
//                     int startIndex = response.indexOf("+CSQ:") + 5;
//                     int commaIndex = response.indexOf(",", startIndex);

//                     if (commaIndex != -1) {
//                         String signalStr = response.substring(startIndex, commaIndex);
//                         signalStr.trim();
//                         return signalStr.toInt();
//                     }
//                 }

//                 // Exit if we get OK or ERROR
//                 if (response.indexOf("OK") != -1 || response.indexOf("ERROR") != -1) {
//                     break;
//                 }
//             }
//         }

//         Serial.println("[GSM] Failed to get signal quality");
//         return -1;
//     }

//     // Check for incoming messages and calls
//     void update() {
//         if (!initialized) return;

//         if (gsmSerial.available() > 0) {
//             String response = readString();

//             // Check for incoming call
//             if (response.indexOf("RING") != -1) {
//                 incomingCall = true;
//                 Serial.println("[GSM] Incoming call detected");
//             }
//             // Check for call connected
//             else if (response.indexOf("+CLCC:") != -1) {
//                 outgoingCallDetected = true;
//                 Serial.println("[GSM] Call in progress");
//             }
//             // Check for incoming SMS
//             else if (response.indexOf("+CMT:") != -1 || response.indexOf("+CMTI:") != -1) {
//                 Serial.println("[GSM] New SMS received!");

//                 // Extract sender if available in CMT format
//                 if (response.indexOf("+CMT:") != -1) {
//                     int startPos = response.indexOf("\"") + 1;
//                     int endPos = response.indexOf("\"", startPos);
//                     if (startPos > 0 && endPos > startPos) {
//                         String sender = response.substring(startPos, endPos);
//                         Serial.print("[GSM] SMS from: ");
//                         Serial.println(sender);
//                     }
//                 }

//                 // Extract message index for CMTI format
//                 if (response.indexOf("+CMTI:") != -1) {
//                     int startPos = response.lastIndexOf(",") + 1;
//                     newSMSIndex = response.substring(startPos);
//                     newSMSIndex.trim();

//                     // Read the message content
//                     readSMS(newSMSIndex);
//                 }
//             }
//         }
//     }

//     // Function to read an SMS by index
//     void readSMS(const String &index) {
//         if (!checkInitialized() || !checkCooldown()) return;

//         Serial.print("[GSM] Reading SMS at index: ");
//         Serial.println(index);

//         // Set SMS text mode
//         ATCommandOK("AT+CMGF=1", true);

//         // Read the message
//         gsmSerial.println("AT+CMGR=" + index);

//         // Wait for response and process it
//         String response = "";
//         String smsContent = "";
//         bool messageHeader = false;
//         unsigned long timeout = millis() + 5000;

//         while (millis() < timeout) {
//             if (gsmSerial.available()) {
//                 char c = gsmSerial.read();
//                 response += c;

//                 // Once we get past the header, capture the message content
//                 if (messageHeader && c == '\n') {
//                     smsContent = "";
//                 } else if (messageHeader) {
//                     smsContent += c;
//                 }

//                 // Detect SMS header
//                 if (response.indexOf("+CMGR:") != -1) {
//                     messageHeader = true;
//                 }

//                 // Exit if we get OK or ERROR
//                 if (response.indexOf("OK") != -1 || response.indexOf("ERROR") != -1) {
//                     break;
//                 }
//             }
//         }

//         if (smsContent.length() > 0) {
//             Serial.print("[GSM] SMS content: ");
//             Serial.println(smsContent);
//         }

//         // Delete the read message
//         ATCommandOK("AT+CMGD=" + index + ",0", true);
//     }

//     // Delete all SMS
//     bool deleteAllSMS() {
//         if (!checkInitialized() || !checkCooldown()) return false;

//         Serial.println("[GSM] Deleting all SMS messages");
//         return ATCommandOK("AT+CMGD=1,4", true); // 1,4 mode deletes all
//     }

//     // Check if module is responding
//     bool isResponding() {
//         if (!checkCooldown()) return false;
//         return ATCommandOK("AT", true);
//     }

//     // General AT command function - exposed publicly for direct command use
//     bool ATCommandOK(String cmd, bool extendTime) {
//         if (!checkCooldown()) return false;

//         Serial.print("[GSM TX] ");
//         Serial.println(cmd);
//         gsmSerial.println(cmd);

//         lastCommandTime = millis();

//         String response = "";
//         unsigned long timeout = millis() + (extendTime ? 10000 : 5000);

//         while (millis() < timeout) {
//             if (gsmSerial.available()) {
//                 char c = gsmSerial.read();
//                 response += c;
//                 Serial.write(c);

//                 // Check if response is complete
//                 if (response.indexOf("OK") != -1) {
//                     Serial.println();
//                     return true;
//                 } else if (response.indexOf("ERROR") != -1) {
//                     Serial.println();
//                     return false;
//                 }
//             }
//         }

//         Serial.println("\n[GSM] Command timeout!");
//         return false;
//     }

// private:
//     // Check if module is initialized
//     bool checkInitialized() {
//         if (!initialized) {
//             Serial.println("[GSM] Error: Module not initialized. Call begin() first.");
//             return false;
//         }
//         return true;
//     }

//     // Check command cooldown to prevent flooding module
//     bool checkCooldown() {
//         if (millis() - lastCommandTime < commandCooldown) {
//             delay(commandCooldown - (millis() - lastCommandTime));
//         }
//         return true;
//     }

//     // Function to send an AT command and wait for '>' response
//     bool sendBlockingATCommand(String cmd, bool extendTime) {
//         if (!checkCooldown()) return false;

//         Serial.print("[GSM TX] ");
//         Serial.println(cmd);
//         gsmSerial.println(cmd);

//         lastCommandTime = millis();

//         String response = "";
//         unsigned long timeout = millis() + (extendTime ? 10000 : 5000);

//         while (millis() < timeout) {
//             if (gsmSerial.available()) {
//                 char c = gsmSerial.read();
//                 response += c;
//                 Serial.write(c);

//                 // Check for prompt or error
//                 if (response.indexOf(">") != -1) {
//                     return true;
//                 } else if (response.indexOf("ERROR") != -1) {
//                     Serial.println();
//                     return false;
//                 }
//             }
//         }

//         Serial.println("\n[GSM] Command timeout (expected '>')");
//         return false;
//     }

//     // Function to read a string from serial
//     String readString() {
//         String str = "";
//         if (gsmSerial.available() > 0) {
//             unsigned long timeout = millis() + 500; // 500ms timeout for complete message

//             while (millis() < timeout && gsmSerial.available()) {
//                 char c = gsmSerial.read();
//                 str += c;

//                 // Extend timeout if we're still receiving
//                 if (gsmSerial.available()) {
//                     timeout = millis() + 100;
//                 }
//             }

//             Serial.print("[GSM RX] ");
//             Serial.println(str);
//         }
//         return str;
//     }
// };

// #endif // GSM_MODULE_H



#ifndef GSM_MODULE_H
#define GSM_MODULE_H

#include <Arduino.h>

class GSMModule {
private:
    HardwareSerial& gsmSerial;
    bool initialized = false;
    bool incomingCall = false;
    bool outgoingCallDetected = false;
    String newSMSIndex;
    unsigned long lastCommandTime = 0;
    const unsigned long commandCooldown = 1000; // 1 second cooldown between commands

public:
    // Constructor
    GSMModule(HardwareSerial& serial) : gsmSerial(serial) {}

    // Initialize GSM module with specified baud rate (default 9600)
    bool begin(unsigned long baudRate = 9600) {
        gsmSerial.begin(baudRate, SERIAL_8N1, 16, 17); // RX=16, TX=17 for ESP32
        delay(3000); // Give module time to startup

        Serial.println("[GSM] Initializing SIM800L module...");

        // Basic AT command test
        if (!ATCommandOK("AT", true)) {
            Serial.println("[GSM] Error: Module not responding to AT commands");
            return false;
        }

        // Turn off echo
        ATCommandOK("ATE0", true);

        // Set SMS to text mode
        if (!ATCommandOK("AT+CMGF=1", true)) {
            Serial.println("[GSM] Error: Failed to set SMS text mode");
            return false;
        }

        // Set new message indications
        ATCommandOK("AT+CNMI=1,2,0,0,0", true);

        // Check if module is registered to network
        if (!ATCommandOK("AT+CREG?", true)) {
            Serial.println("[GSM] Warning: Module not registered to network");
            // Continue anyway as it might register later
        }

        // Check signal quality
        ATCommandOK("AT+CSQ", true);

        initialized = true;
        Serial.println("[GSM] Module initialized successfully");
        return true;
    }

    // Make a phone call
    bool makeCall(const String &phoneNumber) {
        if (!checkInitialized() || !checkCooldown()) return false;

        Serial.print("[GSM] Making call to: ");
        Serial.println(phoneNumber);

        // Reset flags
        incomingCall = false;
        outgoingCallDetected = false;

        // Check network registration
        ATCommandOK("AT+CREG?", true);

        // Set audio settings
        // ATCommandOK("AT+CLVL=5", true); // Set volume level

        // Display detailed dialing info
        Serial.println("[GSM] Dialing with ATD command...");

        // Use direct command instead of ATCommandOK for better monitoring
        // gsmSerial.print("ATD");
        // gsmSerial.print(phoneNumber);
        // gsmSerial.println(";");
        gsmSerial.println("ATD+" + phoneNumber+";");  // No semicolon

        // Monitor the response for a short period
        String dialResponse = "";
        unsigned long dialStart = millis();
        bool callInitiated = false;

        // Look for responses for 5 seconds
        while (millis() - dialStart < 5000) {
            if (gsmSerial.available()) {
                char c = gsmSerial.read();
                dialResponse += c;
                Serial.write(c); // Echo to Serial monitor

                // Look for success indicators
                if (dialResponse.indexOf("OK") != -1) {
                    callInitiated = true;
                    Serial.println("\n[GSM] Call initiation confirmed (OK received)");
                } else if (dialResponse.indexOf("NO CARRIER") != -1) {
                    Serial.println("\n[GSM] Call failed (NO CARRIER)");
                    return false;
                } else if (dialResponse.indexOf("ERROR") != -1) {
                    Serial.println("\n[GSM] Call failed (ERROR received)");
                    return false;
                } else if (dialResponse.indexOf("BUSY") != -1) {
                    Serial.println("\n[GSM] Number is busy");
                    return false;
                }
            }
        }

        if (!callInitiated) {
            Serial.println("\n[GSM] Warning: No confirmation for call initiation");
        }

        // Let the call ring for some time (20 seconds)
        Serial.println("[GSM] Call in progress, waiting 20 seconds...");

        // Monitor call status while waiting
        unsigned long callStart = millis();
        bool callConnected = false;

        // Check for call status during the 20-second period
        while (millis() - callStart < 20000) {
            if (gsmSerial.available()) {
                char c = gsmSerial.read();
                Serial.write(c); // Echo everything to Serial monitor

                // Optional: Look for specific call status indicators
            }

            // Brief pause to avoid hogging CPU
            delay(100);
        }

        // Hang up the call
        Serial.println("\n[GSM] Hanging up call");
        ATCommandOK("ATH", true);
        Serial.println("[GSM] Call ended");
        return true;
    }

    // Make multiple calls to a phone number with delays in between
    bool makeMultipleCalls(const String &phoneNumber, int numCalls, int delayBetweenCalls) {
        if (!checkInitialized()) return false;

        Serial.print("[GSM] Making ");
        Serial.print(numCalls);
        Serial.print(" calls to: ");
        Serial.println(phoneNumber);

        bool allCallsSucceeded = true;

        for (int i = 0; i < numCalls; i++) {
            // Check signal strength before making the call
            int signalStrength = getSignalQuality();

            if (signalStrength >= 0) {
                Serial.print("[GSM] Current signal strength: ");
                Serial.print(signalStrength);
                Serial.println(" (0-31, higher is better)");

                // Evaluate signal quality
                if (signalStrength == 99) {
                    Serial.println("[GSM] Warning: Signal strength unknown");
                } else if (signalStrength < 10) {
                    Serial.println("[GSM] Warning: Signal strength is low, call might fail");
                } else if (signalStrength >= 20) {
                    Serial.println("[GSM] Signal strength is good");
                } else {
                    Serial.println("[GSM] Signal strength is acceptable");
                }
            } else {
                Serial.println("[GSM] Failed to get signal strength");
            }

            Serial.print("[GSM] Call ");
            Serial.print(i + 1);
            Serial.print(" of ");
            Serial.println(numCalls);

            if (!makeCall(phoneNumber)) {
                Serial.print("[GSM] Call ");
                Serial.print(i + 1);
                Serial.println(" failed");
                allCallsSucceeded = false;
            }

            // Wait between calls if not the last call
            if (i < numCalls - 1) {
                Serial.print("[GSM] Waiting ");
                Serial.print(delayBetweenCalls / 1000);
                Serial.println(" seconds before next call");
                delay(delayBetweenCalls);
            }
        }

        Serial.println("[GSM] Multiple call sequence completed");
        return allCallsSucceeded;
    }

    // Send an SMS
    bool sendSMS(const String &phoneNumber, const String &message) {
        if (!checkInitialized() || !checkCooldown()) return false;

        Serial.print("[GSM] Sending SMS to: ");
        Serial.println(phoneNumber);

        // Set SMS text mode
        if (!ATCommandOK("AT+CMGF=1", true)) {
            Serial.println("[GSM] Failed to set SMS text mode");
            return false;
        }

        // Set recipient
        if (!sendBlockingATCommand("AT+CMGS=\"" + phoneNumber + "\"", true)) {
            Serial.println("[GSM] Failed to set recipient number");
            return false;
        }

        // Send message content
        gsmSerial.print(message);
        delay(500);

        // Send Ctrl+Z to finalize
        gsmSerial.write(26);

        // Wait for message to send
        delay(5000);

        Serial.println("[GSM] SMS sent successfully");
        return true;
    }

    // Hang up any active call
    bool hangUp() {
        if (!checkInitialized() || !checkCooldown()) return false;

        Serial.println("[GSM] Hanging up call");
        return ATCommandOK("ATH", true);
    }

    // Get signal quality (0-31, 99=unknown)
    int getSignalQuality() {
        if (!checkInitialized() || !checkCooldown()) return -1;

        Serial.println("[GSM] Checking signal quality...");
        gsmSerial.println("AT+CSQ");

        String response = "";
        unsigned long timeout = millis() + 5000;
        int signalQuality = -1;

        while (millis() < timeout) {
            if (gsmSerial.available()) {
                char c = gsmSerial.read();
                response += c;
                Serial.write(c); // Echo to Serial monitor for debugging

                // Look for signal quality response
                if (response.indexOf("+CSQ:") != -1) {
                    int startIndex = response.indexOf("+CSQ:") + 5;
                    int commaIndex = response.indexOf(",", startIndex);

                    if (commaIndex != -1) {
                        String signalStr = response.substring(startIndex, commaIndex);
                        signalStr.trim();
                        signalQuality = signalStr.toInt();
                    }
                }

                // Exit if we get OK or ERROR
                if (response.indexOf("OK") != -1 || response.indexOf("ERROR") != -1) {
                    break;
                }
            }
        }

        if (signalQuality >= 0) {
            Serial.print("[GSM] Signal quality: ");
            Serial.print(signalQuality);

            // Interpret the signal quality value
            if (signalQuality == 99) {
                Serial.println(" (Unknown signal)");
            } else if (signalQuality == 0) {
                Serial.println(" (No signal, -115 dBm or less)");
            } else if (signalQuality <= 9) {
                Serial.println(" (Marginal signal, -95 to -105 dBm)");
            } else if (signalQuality <= 14) {
                Serial.println(" (OK signal, -85 to -95 dBm)");
            } else if (signalQuality <= 19) {
                Serial.println(" (Good signal, -75 to -85 dBm)");
            } else if (signalQuality <= 31) {
                Serial.println(" (Excellent signal, -75 dBm or better)");
            }
        } else {
            Serial.println("[GSM] Failed to get signal quality");
        }

        return signalQuality;
    }

    // Check for incoming messages and calls
    void update() {
        if (!initialized) return;

        if (gsmSerial.available() > 0) {
            String response = readString();

            // Check for incoming call
            if (response.indexOf("RING") != -1) {
                incomingCall = true;
                Serial.println("[GSM] Incoming call detected");
            }
            // Check for call connected
            else if (response.indexOf("+CLCC:") != -1) {
                outgoingCallDetected = true;
                Serial.println("[GSM] Call in progress");
            }
            // Check for incoming SMS
            else if (response.indexOf("+CMT:") != -1 || response.indexOf("+CMTI:") != -1) {
                Serial.println("[GSM] New SMS received!");

                // Extract sender if available in CMT format
                if (response.indexOf("+CMT:") != -1) {
                    int startPos = response.indexOf("\"") + 1;
                    int endPos = response.indexOf("\"", startPos);
                    if (startPos > 0 && endPos > startPos) {
                        String sender = response.substring(startPos, endPos);
                        Serial.print("[GSM] SMS from: ");
                        Serial.println(sender);
                    }
                }

                // Extract message index for CMTI format
                if (response.indexOf("+CMTI:") != -1) {
                    int startPos = response.lastIndexOf(",") + 1;
                    newSMSIndex = response.substring(startPos);
                    newSMSIndex.trim();

                    // Read the message content
                    readSMS(newSMSIndex);
                }
            }
        }
    }

    // Function to read an SMS by index
    void readSMS(const String &index) {
        if (!checkInitialized() || !checkCooldown()) return;

        Serial.print("[GSM] Reading SMS at index: ");
        Serial.println(index);

        // Set SMS text mode
        ATCommandOK("AT+CMGF=1", true);

        // Read the message
        gsmSerial.println("AT+CMGR=" + index);

        // Wait for response and process it
        String response = "";
        String smsContent = "";
        bool messageHeader = false;
        unsigned long timeout = millis() + 5000;

        while (millis() < timeout) {
            if (gsmSerial.available()) {
                char c = gsmSerial.read();
                response += c;

                // Once we get past the header, capture the message content
                if (messageHeader && c == '\n') {
                    smsContent = "";
                } else if (messageHeader) {
                    smsContent += c;
                }

                // Detect SMS header
                if (response.indexOf("+CMGR:") != -1) {
                    messageHeader = true;
                }

                // Exit if we get OK or ERROR
                if (response.indexOf("OK") != -1 || response.indexOf("ERROR") != -1) {
                    break;
                }
            }
        }

        if (smsContent.length() > 0) {
            Serial.print("[GSM] SMS content: ");
            Serial.println(smsContent);
        }

        // Delete the read message
        ATCommandOK("AT+CMGD=" + index + ",0", true);
    }

    // Delete all SMS
    bool deleteAllSMS() {
        if (!checkInitialized() || !checkCooldown()) return false;

        Serial.println("[GSM] Deleting all SMS messages");
        return ATCommandOK("AT+CMGD=1,4", true); // 1,4 mode deletes all
    }

    // Check if module is responding
    bool isResponding() {
        if (!checkCooldown()) return false;
        return ATCommandOK("AT", true);
    }

    // General AT command function - exposed publicly for direct command use
    bool ATCommandOK(String cmd, bool extendTime) {
        if (!checkCooldown()) return false;

        Serial.print("[GSM TX] ");
        Serial.println(cmd);
        gsmSerial.println(cmd);

        lastCommandTime = millis();

        String response = "";
        unsigned long timeout = millis() + (extendTime ? 10000 : 5000);

        while (millis() < timeout) {
            if (gsmSerial.available()) {
                char c = gsmSerial.read();
                response += c;
                Serial.write(c);

                // Check if response is complete
                if (response.indexOf("OK") != -1) {
                    Serial.println();
                    return true;
                } else if (response.indexOf("ERROR") != -1) {
                    Serial.println();
                    return false;
                }
            }
        }

        Serial.println("\n[GSM] Command timeout!");
        return false;
    }

private:
    // Check if module is initialized
    bool checkInitialized() {
        if (!initialized) {
            Serial.println("[GSM] Error: Module not initialized. Call begin() first.");
            return false;
        }
        return true;
    }

    // Check command cooldown to prevent flooding module
    bool checkCooldown() {
        if (millis() - lastCommandTime < commandCooldown) {
            delay(commandCooldown - (millis() - lastCommandTime));
        }
        return true;
    }

    // Function to send an AT command and wait for '>' response
    bool sendBlockingATCommand(String cmd, bool extendTime) {
        if (!checkCooldown()) return false;

        Serial.print("[GSM TX] ");
        Serial.println(cmd);
        gsmSerial.println(cmd);

        lastCommandTime = millis();

        String response = "";
        unsigned long timeout = millis() + (extendTime ? 10000 : 5000);

        while (millis() < timeout) {
            if (gsmSerial.available()) {
                char c = gsmSerial.read();
                response += c;
                Serial.write(c);

                // Check for prompt or error
                if (response.indexOf(">") != -1) {
                    return true;
                } else if (response.indexOf("ERROR") != -1) {
                    Serial.println();
                    return false;
                }
            }
        }

        Serial.println("\n[GSM] Command timeout (expected '>')");
        return false;
    }

    // Function to read a string from serial
    String readString() {
        String str = "";
        if (gsmSerial.available() > 0) {
            unsigned long timeout = millis() + 500; // 500ms timeout for complete message

            while (millis() < timeout && gsmSerial.available()) {
                char c = gsmSerial.read();
                str += c;

                // Extend timeout if we're still receiving
                if (gsmSerial.available()) {
                    timeout = millis() + 100;
                }
            }

            Serial.print("[GSM RX] ");
            Serial.println(str);
        }
        return str;
    }
};

#endif // GSM_MODULE_H