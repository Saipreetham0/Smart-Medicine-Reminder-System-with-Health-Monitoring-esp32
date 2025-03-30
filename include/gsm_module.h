#ifndef GSM_MODULE_H
#define GSM_MODULE_H

#include <Arduino.h>

class GSMModule {
private:
    HardwareSerial& gsmSerial;
    bool baud = false;
    bool incomingCall = false;
    bool outgoingCallDetected = false;
    String newSMSIndex;

public:
    // Constructor
    GSMModule(HardwareSerial& serial) : gsmSerial(serial) {}

    // Initialize GSM module
    void begin(unsigned long baudRate) {
        gsmSerial.begin(baudRate, SERIAL_8N1, 17, 16);  // 4G Module on pins 17 (RX) and 16 (TX)
        delay(1000);

        if (!baud) {
            ATCommandOK("AT+IPR=19200\r\n", true);  // Set baud rate
            gsmSerial.end();
            gsmSerial.begin(19200, SERIAL_8N1, 17, 16);
            baud = true;
            Serial.println("GSM module initialized");
        }

        // Setup SMS text mode
        ATCommandOK("AT+CMGF=1\r\n", true);
        // Setup new message indications
        ATCommandOK("AT+CNMI=1,2,0,0,0\r\n", true);
    }

    // Make a phone call
    void makeCall(const String &phoneNumber) {
        Serial.print("[GSM] Making call to: ");
        Serial.println(phoneNumber);

        // Reset flags
        incomingCall = false;
        outgoingCallDetected = false;

        // Command to make a call
        ATCommandOK("ATD" + phoneNumber + ";\r\n", true);

        // Wait for call to establish
        delay(1000);

        // Let the call ring for some time
        delay(20000);

        // Hang up the call
        ATCommandOK("ATH\r\n", true);
        Serial.println("[GSM] Call ended");
    }

    // Send an SMS
    void sendSMS(const String &phoneNumber, const String &message) {
        Serial.print("[GSM] Sending SMS to: ");
        Serial.println(phoneNumber);

        // Set SMS text mode
        ATCommandOK("AT+CMGF=1\r\n", true);

        // Set SMS recipient
        if (sendBlockingATCommand("AT+CMGS=\"" + phoneNumber + "\"\r\n", true)) {
            // Send the message
            gsmSerial.println(message);

            // Ctrl+Z to send the message
            gsmSerial.write(26);

            // Wait for the SMS to be sent
            delay(2000);

            Serial.println("[GSM] SMS sent");
        } else {
            Serial.println("[GSM] Failed to send SMS");
        }
    }

    // Attend to an incoming call
    void attendCall() {
        Serial.println("[GSM] Attending call...");
        ATCommandOK("ATA\r\n", true);
    }

    // Check for incoming messages and calls
    void update() {
        if (gsmSerial.available() > 0) {
            String response = readString();

            // Check for incoming call
            if (response.indexOf("+CLCC:") != -1) {
                if (!outgoingCallDetected) {
                    outgoingCallDetected = true;
                    Serial.println("[GSM] Call in progress");
                }
            }
            // Check for incoming SMS
            else if (response.indexOf("+CMTI") != -1) {
                Serial.println("[GSM] New SMS received!");

                // Extract index of the new SMS
                int startPos = response.indexOf(",") + 1;
                int endPos = response.indexOf("\",", startPos);
                if (endPos == -1) endPos = response.length();

                newSMSIndex = response.substring(startPos, endPos);
                Serial.print("[GSM] Index of new SMS: ");
                Serial.println(newSMSIndex);

                // Read the content of the new SMS
                ATCommandOK("AT+CMGF=1\r\n", true);
                ATCommandOK("AT+CMGR=" + newSMSIndex + "\r\n", true);

                // Delete the read SMS
                ATCommandOK("AT+CMGD=1,4\r\n", true);
            }
        }
    }

private:
    // Function to send an AT command and wait for 'OK' or 'ERROR' response
    bool ATCommandOK(String cmd, bool extendTime) {
        Serial.print("[GSM TX] ");
        Serial.print(cmd);
        gsmSerial.print(cmd);

        unsigned long t = millis();
        String str;
        unsigned long int t2 = extendTime ? 6000L : 4000L;  // Timeout: 4 or 6 seconds

        while (millis() - t < t2) {
            if (gsmSerial.available() > 0) {
                str = readString();
                if (str.indexOf("OK") != -1)
                    return true;  // 'OK' received
                else if (str.indexOf("ERROR") != -1)
                    return false;  // 'ERROR' received
            }
        }
        Serial.println("[GSM] Command timeout");
        return false;  // Timeout occurred
    }

    // Function to send an AT command and wait for '>' response
    bool sendBlockingATCommand(String cmd, bool extendTime) {
        Serial.print("[GSM TX] ");
        Serial.print(cmd);
        gsmSerial.print(cmd);

        unsigned long t = millis();
        String str;
        unsigned long int t2 = extendTime ? 6000L : 4000L;  // Timeout: 4 or 6 seconds

        while (millis() - t < t2) {
            if (gsmSerial.available() > 0) {
                str = readString();
                if (str.indexOf(">") != -1)
                    return true;  // '>' received
                else if (str.indexOf("ERROR") != -1)
                    return false;  // 'ERROR' received
            }
        }
        Serial.println("[GSM] Command timeout (expected '>')");
        return false;  // Timeout occurred
    }

    // Function to read a string from serial
    String readString() {
        String str = "";
        if (gsmSerial.available() > 0) {
            str = gsmSerial.readStringUntil('\n');
            Serial.print("[GSM RX] ");
            Serial.println(str);
        }
        return str;
    }
};

#endif // GSM_MODULE_H