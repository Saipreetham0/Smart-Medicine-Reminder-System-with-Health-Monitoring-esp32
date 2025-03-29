#ifndef GSM_MODULE_H
#define GSM_MODULE_H

#include <Arduino.h>
#include "SoftwareSerial.h"

class GSMModule {
private:
    SoftwareSerial* gsmSerial;

public:
    // Constructor
    GSMModule(int rxPin, int txPin) {
        gsmSerial = new SoftwareSerial(rxPin, txPin);
    }

    // Destructor
    ~GSMModule() {
        delete gsmSerial;
    }

    // Initialize GSM module
    void begin(int baudRate = 9600) {
        gsmSerial->begin(baudRate);
        delay(1000);

        // Initialize GSM module
        sendATCommand("AT", 1000);
        sendATCommand("AT+CMGF=1", 1000);  // Set SMS text mode
        sendATCommand("AT+CNMI=1,2,0,0,0", 1000);  // Set new message indications
    }

    // Send AT command to GSM module
    String sendATCommand(const String &command, int timeout) {
        gsmSerial->println(command);
        String response = "";

        long int time = millis();
        while((time + timeout) > millis()) {
            while(gsmSerial->available()) {
                response += (char)gsmSerial->read();
            }
        }

        Serial.print("GSM Response: ");
        Serial.println(response);
        return response;
    }

    // Make a phone call
    void makeCall(const String &phoneNumber) {
        Serial.print("Making call to: ");
        Serial.println(phoneNumber);

        String callCommand = "ATD" + phoneNumber + ";";
        sendATCommand(callCommand, 1000);

        // Let the call ring for 20 seconds
        delay(20000);

        // Hang up
        sendATCommand("ATH", 1000);
    }

    // Send SMS
    void sendSMS(const String &phoneNumber, const String &message) {
        Serial.print("Sending SMS to: ");
        Serial.println(phoneNumber);

        sendATCommand("AT+CMGF=1", 1000);  // Set SMS text mode
        delay(100);

        String smsCommand = "AT+CMGS=\"" + phoneNumber + "\"";
        gsmSerial->println(smsCommand);
        delay(100);

        gsmSerial->println(message);
        delay(100);

        gsmSerial->write(26);  // Ctrl+Z to end SMS
        delay(1000);
    }
};

#endif // GSM_MODULE_H