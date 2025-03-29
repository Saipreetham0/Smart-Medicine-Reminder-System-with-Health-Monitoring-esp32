#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <Arduino.h>

// Medicine Schedule Structure
struct MedicineSchedule {
  int chamberIndex;
  int hour;
  int minute;
  bool taken;
  unsigned long alarmStartTime;
  bool alarmActive;
  bool callSent;

  // Constructor with default values
  MedicineSchedule() :
    chamberIndex(0),
    hour(0),
    minute(0),
    taken(false),
    alarmStartTime(0),
    alarmActive(false),
    callSent(false) {}
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