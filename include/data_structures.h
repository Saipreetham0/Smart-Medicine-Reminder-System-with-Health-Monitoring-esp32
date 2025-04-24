


#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <Arduino.h>

// Medicine Schedule Structure
struct MedicineSchedule {
  int chamberIndex;           // Which medicine chamber (0-7)
  int hour;                   // Hour for the schedule (0-23)
  int minute;                 // Minute for the schedule (0-59)
  bool taken;                 // Has the medicine been taken?
  bool alarmActive;           // Is the alarm currently active?
  bool callSent;              // Has a reminder call been sent?
  unsigned long alarmStartTime; // When was the alarm activated

  // Constructor with default values
  MedicineSchedule() :
    chamberIndex(0),
    hour(0),
    minute(0),
    taken(false),
    alarmActive(false),
    callSent(false),
    alarmStartTime(0) {}
};

// User Structure
struct User {
  String username;
  String password;
  String role;
  String phone;

  // Constructor with default values
  User() :
    username(""),
    password(""),
    role(""),
    phone("") {}
};

#endif // DATA_STRUCTURES_H