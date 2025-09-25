# Smart Medicine Reminder System with Health Monitoring

## 🏥 Overview

An intelligent ESP32-based medicine reminder system that helps users manage their medication schedules with automated reminders, health monitoring, and emergency notifications. The system features an 8-chamber medicine dispenser with LED indicators, buzzer alerts, GSM connectivity for SMS/call notifications, and a web-based interface for easy management.

## ✨ Features

### Medicine Management
- **8-Chamber Medicine Dispenser** - Organized storage for different medications
- **LED Indicators** - Visual cues for each medicine chamber
- **Reed Switch Detection** - Confirms when medicines are taken
- **Buzzer Alerts** - Audio reminders for medication times

### Smart Reminders
- **Scheduled Notifications** - Set custom reminder times for each medicine
- **Progressive Alert System** - Escalating notifications if medicine is not taken
- **SMS Notifications** - Text message reminders via GSM module
- **Emergency Calls** - Automatic calls for critical missed medications

### Connectivity & Interface
- **WiFi Access Point** - Direct connection for device management
- **Web Interface** - User-friendly browser-based control panel
- **Real-Time Clock** - Accurate timekeeping for schedules
- **LittleFS Storage** - Persistent data storage for schedules and settings

### Health Monitoring
- **Medicine Adherence Tracking** - Monitor medication compliance
- **Schedule Management** - Add, edit, and delete medicine schedules
- **Status Monitoring** - Real-time system and battery status

## 🛠️ Hardware Requirements

### Main Components
- **ESP32 Development Board** (ESP32-DOIT-DEVKIT-V1)
- **RTC Module** (DS3231 or DS1307)
- **GSM Module** (SIM800L or similar)
- **Buzzer** (Active or Passive)
- **8x LEDs** (for chamber indicators)
- **Reed Switches** (magnetic door sensors)
- **Medicine Storage Chambers** (8 compartments)

### Pin Configuration
```
LED Pins: 13, 12, 14, 27, 26, 25, 33, 32
Reed Switch: Pin 19
Buzzer: Pin 5
GSM Module: RX-16, TX-17
I2C (RTC): SDA-21, SCL-22
```

## 📱 Software Features

### Web Interface
- Schedule medicine reminders
- Monitor system status
- View medicine adherence history
- Configure emergency contacts
- System settings management

### GSM Functionality
- SMS reminder notifications
- Emergency call system
- Status update messages
- Network connectivity monitoring

## 🚀 Getting Started

### Prerequisites
- [PlatformIO](https://platformio.org/) development environment
- ESP32 development board
- Required hardware components (see Hardware Requirements)

### Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/Saipreetham0/Smart-Medicine-Reminder-System-with-Health-Monitoring-esp32.git
   cd Smart-Medicine-Reminder-System-with-Health-Monitoring-esp32
   ```

2. **Open in PlatformIO**
   - Open the project folder in PlatformIO IDE
   - All dependencies will be automatically downloaded

3. **Hardware Setup**
   - Connect components according to pin configuration
   - Ensure proper power supply for all modules
   - Insert SIM card in GSM module

4. **Upload Code**
   ```bash
   pio run --target upload
   ```

5. **Access Web Interface**
   - Connect to WiFi AP: "Medicine Reminder"
   - Password: "medkit123"
   - Open browser to: `http://192.168.4.1`

## 🔧 Configuration

### WiFi Settings
```cpp
const char* ssid = "Medicine Reminder";
const char* password = "medkit123";
```

### GSM Configuration
- Insert active SIM card
- Configure emergency contact numbers
- Set SMS service center number

### Medicine Schedules
- Use web interface to add schedules
- Set chamber number (1-8)
- Configure reminder times
- Enable/disable emergency calls

## 📊 System Architecture

The system is built with modular design:

- **`main.cpp`** - Main application logic and setup
- **`data_structures.h`** - Core data structures for schedules and users
- **`file_storage.h`** - LittleFS file operations
- **`web_server.h`** - HTTP server and API endpoints
- **`gsm_module.h`** - GSM communication functions
- **`html_assets.h`** - Web interface HTML/CSS/JS

## 🛡️ Safety Features

- **Fail-safe Operation** - System continues working even if WiFi/GSM fails
- **Battery Backup** - RTC maintains time during power outages
- **Data Persistence** - Schedules saved to flash memory
- **Emergency Protocols** - Automatic escalation for critical medicines

## 🛒 Commercial Availability

**This project is available for purchase at [kspelectronics.in](https://kspelectronics.in)**

Complete kits include:
- Pre-programmed ESP32 board
- All required hardware components
- Assembly instructions
- Technical support

## 🤝 Contributing

This is an open-source project. Contributions are welcome!

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## 📄 License

This project is open source and available under the MIT License.

## 👨‍💻 Author

**Sai Preetham Koyyala**
- GitHub: [@Saipreetham0](https://github.com/Saipreetham0)
- Website: [kspelectronics.in](https://kspelectronics.in)

## 📞 Support

For technical support or commercial inquiries:
- Visit: [kspelectronics.in](https://kspelectronics.in)
- Email: Contact through website
- GitHub Issues: For bug reports and feature requests

## 🔮 Future Enhancements

- Mobile application for remote monitoring
- IoT cloud integration
- Advanced health analytics
- Multi-user support
- Voice command integration
- Bluetooth connectivity

---

**⚡ Powered by ESP32 | 💊 Healthcare Technology | 🏠 Smart Home Innovation**