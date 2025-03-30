

// #ifndef HTML_ASSETS_H
// #define HTML_ASSETS_H

// #include <Arduino.h>

// // Login HTML page - With simple authentication
// const char LOGIN_HTML[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html lang="en">
// <head>
//     <meta charset="UTF-8">
//     <meta name="viewport" content="width=device-width, initial-scale=1.0">
//     <title>Smart Medicine Reminder</title>
//     <style>
//         body {
//             font-family: Arial, sans-serif;
//             background-color: #f0f7ff;
//             margin: 0;
//             padding: 0;
//             display: flex;
//             justify-content: center;
//             align-items: center;
//             height: 100vh;
//         }

//         .card {
//             background-color: white;
//             border-radius: 10px;
//             box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
//             padding: 30px;
//             width: 350px;
//             text-align: center;
//         }

//         h1 {
//             color: #1a73e8;
//             margin-bottom: 5px;
//         }

//         .subtitle {
//             color: #5f6368;
//             margin-bottom: 30px;
//         }

//         .form-group {
//             margin-bottom: 15px;
//             text-align: left;
//         }

//         label {
//             display: block;
//             margin-bottom: 5px;
//             font-weight: 500;
//             color: #5f6368;
//         }

//         input {
//             width: 100%;
//             padding: 10px;
//             border: 1px solid #dadce0;
//             border-radius: 4px;
//             box-sizing: border-box;
//             font-size: 14px;
//         }

//         .button {
//             background-color: #1a73e8;
//             color: white;
//             border: none;
//             border-radius: 4px;
//             padding: 12px;
//             width: 100%;
//             font-size: 14px;
//             cursor: pointer;
//             margin-top: 10px;
//             font-weight: 500;
//         }

//         .note {
//             margin-top: 15px;
//             font-size: 12px;
//             color: #5f6368;
//         }

//         .error {
//             color: #d93025;
//             margin-top: 10px;
//             display: none;
//         }
//     </style>
// </head>
// <body>
//     <div class="card">
//         <h1>Smart Medicine Reminder</h1>
//         <p class="subtitle">Health Monitoring System for Senior Citizens</p>

//         <div class="form-group">
//             <label for="username">Username</label>
//             <input type="text" id="username" placeholder="Enter username" value="admin">
//         </div>

//         <div class="form-group">
//             <label for="password">Password</label>
//             <input type="password" id="password" placeholder="Enter password" value="admin">
//         </div>

//         <button class="button" id="login-button">Login</button>

//         <div id="error-message" class="error">Invalid username or password</div>

//         <p class="note">Default login: username "admin", password "admin"</p>
//     </div>

//     <script>
//         // Simple login authentication
//         document.getElementById('login-button').addEventListener('click', function() {
//             const username = document.getElementById('username').value;
//             const password = document.getElementById('password').value;

//             if (username === 'admin' && password === 'admin') {
//                 // Successful login
//                 window.location.href = '/dashboard';
//             } else {
//                 // Failed login
//                 document.getElementById('error-message').style.display = 'block';
//             }
//         });
//     </script>
// </body>
// </html>
// )rawliteral";

// // Dashboard HTML page remains the same
// const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html lang="en">
// <head>
//     <meta charset="UTF-8">
//     <meta name="viewport" content="width=device-width, initial-scale=1.0">
//     <title>Medicine Reminder Dashboard</title>
//     <style>
//         body {
//             font-family: Arial, sans-serif;
//             background-color: #f5f5f5;
//             margin: 0;
//             padding: 20px;
//         }

//         .container {
//             max-width: 800px;
//             margin: 0 auto;
//         }

//         h1 {
//             color: #1a73e8;
//         }

//         .card {
//             background-color: white;
//             border-radius: 8px;
//             box-shadow: 0 2px 6px rgba(0,0,0,0.1);
//             padding: 20px;
//             margin-bottom: 20px;
//         }

//         h2 {
//             color: #202124;
//             margin-top: 0;
//         }

//         .form-row {
//             display: flex;
//             margin-bottom: 15px;
//             gap: 15px;
//         }

//         .form-group {
//             flex: 1;
//         }

//         label {
//             display: block;
//             margin-bottom: 5px;
//             font-weight: 500;
//         }

//         input, select {
//             width: 100%;
//             padding: 8px;
//             border: 1px solid #dadce0;
//             border-radius: 4px;
//             box-sizing: border-box;
//         }

//         .button {
//             background-color: #1a73e8;
//             color: white;
//             border: none;
//             border-radius: 4px;
//             padding: 10px 15px;
//             cursor: pointer;
//             font-weight: 500;
//         }

//         table {
//             width: 100%;
//             border-collapse: collapse;
//             margin-top: 15px;
//         }

//         th, td {
//             border: 1px solid #e0e0e0;
//             padding: 10px;
//             text-align: left;
//         }

//         th {
//             background-color: #f8f9fa;
//         }

//         .action-button {
//             background-color: #f44336;
//             color: white;
//             border: none;
//             border-radius: 4px;
//             padding: 5px 10px;
//             cursor: pointer;
//         }

//         .status {
//             padding: 10px;
//             margin-top: 10px;
//             border-radius: 4px;
//             display: none;
//         }

//         .success {
//             background-color: #d4edda;
//             color: #155724;
//             display: block;
//         }

//         .error {
//             background-color: #f8d7da;
//             color: #721c24;
//             display: block;
//         }

//         .logout-button {
//             background-color: #f1f3f4;
//             color: #5f6368;
//             border: none;
//             border-radius: 4px;
//             padding: 8px 15px;
//             cursor: pointer;
//             float: right;
//             margin-top: 10px;
//         }
//     </style>
// </head>
// <body>
//     <div class="container">
//         <h1>Medicine Reminder Dashboard</h1>
//         <button class="logout-button" onclick="window.location.href='/'">Logout</button>

//         <div class="card">
//             <h2>Patient Phone Number</h2>
//             <div class="form-row">
//                 <div class="form-group">
//                     <label for="patient-phone">Phone Number</label>
//                     <input type="tel" id="patient-phone" placeholder="Enter patient's phone number">
//                 </div>
//                 <div class="form-group" style="flex: 0.3; display: flex; align-items: flex-end;">
//                     <button class="button" id="save-phone-button">Save</button>
//                 </div>
//             </div>
//             <div id="phone-status" class="status"></div>
//         </div>

//         <div class="card">
//             <h2>Add Medicine Schedule</h2>
//             <div class="form-row">
//                 <div class="form-group">
//                     <label for="chamber-select">Chamber</label>
//                     <select id="chamber-select">
//                         <option value="0">Chamber 1</option>
//                         <option value="1">Chamber 2</option>
//                         <option value="2">Chamber 3</option>
//                         <option value="3">Chamber 4</option>
//                         <option value="4">Chamber 5</option>
//                         <option value="5">Chamber 6</option>
//                         <option value="6">Chamber 7</option>
//                         <option value="7">Chamber 8</option>
//                     </select>
//                 </div>
//                 <div class="form-group">
//                     <label for="hour-select">Hour</label>
//                     <select id="hour-select">
//                         <option value="0">00</option>
//                         <option value="1">01</option>
//                         <option value="2">02</option>
//                         <option value="3">03</option>
//                         <option value="4">04</option>
//                         <option value="5">05</option>
//                         <option value="6">06</option>
//                         <option value="7">07</option>
//                         <option value="8">08</option>
//                         <option value="9">09</option>
//                         <option value="10">10</option>
//                         <option value="11">11</option>
//                         <option value="12">12</option>
//                         <option value="13">13</option>
//                         <option value="14">14</option>
//                         <option value="15">15</option>
//                         <option value="16">16</option>
//                         <option value="17">17</option>
//                         <option value="18">18</option>
//                         <option value="19">19</option>
//                         <option value="20">20</option>
//                         <option value="21">21</option>
//                         <option value="22">22</option>
//                         <option value="23">23</option>
//                     </select>
//                 </div>
//                 <div class="form-group">
//                     <label for="minute-select">Minute</label>
//                     <select id="minute-select">
//                         <option value="0">00</option>
//                         <option value="5">05</option>
//                         <option value="10">10</option>
//                         <option value="15">15</option>
//                         <option value="20">20</option>
//                         <option value="25">25</option>
//                         <option value="30">30</option>
//                         <option value="35">35</option>
//                         <option value="40">40</option>
//                         <option value="45">45</option>
//                         <option value="50">50</option>
//                         <option value="55">55</option>
//                     </select>
//                 </div>
//                 <div class="form-group" style="flex: 0.3; display: flex; align-items: flex-end;">
//                     <button class="button" id="add-schedule-button">Add</button>
//                 </div>
//             </div>
//             <div id="schedule-status" class="status"></div>

//             <h3>Current Schedules</h3>
//             <table id="schedule-table">
//                 <thead>
//                     <tr>
//                         <th>Chamber</th>
//                         <th>Time</th>
//                         <th>Status</th>
//                         <th>Action</th>
//                     </tr>
//                 </thead>
//                 <tbody id="schedule-body">
//                     <!-- Schedule items will be added here -->
//                 </tbody>
//             </table>
//         </div>
//     </div>

//     <script>
//         // Load data on page load
//         document.addEventListener('DOMContentLoaded', function() {
//             loadPhone();
//             loadSchedules();
//         });

//         // Load patient phone number
//         function loadPhone() {
//             fetch('/get_phone')
//                 .then(response => response.json())
//                 .then(data => {
//                     document.getElementById('patient-phone').value = data.phone;
//                 })
//                 .catch(error => {
//                     console.error('Error loading phone:', error);
//                     showStatus('phone-status', 'Failed to load phone number', false);
//                 });
//         }

//         // Save patient phone number
//         document.getElementById('save-phone-button').addEventListener('click', function() {
//             const phone = document.getElementById('patient-phone').value;

//             fetch('/set_phone', {
//                 method: 'POST',
//                 headers: {
//                     'Content-Type': 'application/x-www-form-urlencoded',
//                 },
//                 body: `phone=${phone}`
//             })
//             .then(response => response.json())
//             .then(data => {
//                 if (data.status === 'success') {
//                     showStatus('phone-status', 'Phone number saved successfully!', true);
//                 } else {
//                     showStatus('phone-status', 'Failed to save phone number', false);
//                 }
//             })
//             .catch(error => {
//                 console.error('Error:', error);
//                 showStatus('phone-status', 'An error occurred while saving phone number', false);
//             });
//         });

//         // Load schedules
//         function loadSchedules() {
//             fetch('/get_schedules')
//                 .then(response => response.json())
//                 .then(data => {
//                     const scheduleBody = document.getElementById('schedule-body');
//                     scheduleBody.innerHTML = '';

//                     if (!data.schedules || data.schedules.length === 0) {
//                         // No schedules
//                         const row = document.createElement('tr');
//                         const cell = document.createElement('td');
//                         cell.textContent = 'No schedules found';
//                         cell.colSpan = 4;
//                         cell.style.textAlign = 'center';
//                         row.appendChild(cell);
//                         scheduleBody.appendChild(row);
//                         return;
//                     }

//                     data.schedules.forEach((schedule, index) => {
//                         const row = document.createElement('tr');

//                         // Chamber
//                         const chamberCell = document.createElement('td');
//                         chamberCell.textContent = `Chamber ${schedule.chamber + 1}`;
//                         row.appendChild(chamberCell);

//                         // Time
//                         const timeCell = document.createElement('td');
//                         const hour = schedule.hour.toString().padStart(2, '0');
//                         const minute = schedule.minute.toString().padStart(2, '0');
//                         timeCell.textContent = `${hour}:${minute}`;
//                         row.appendChild(timeCell);

//                         // Status
//                         const statusCell = document.createElement('td');
//                         statusCell.textContent = schedule.taken ? 'Taken' : 'Pending';
//                         row.appendChild(statusCell);

//                         // Action
//                         const actionCell = document.createElement('td');
//                         const deleteButton = document.createElement('button');
//                         deleteButton.textContent = 'Delete';
//                         deleteButton.className = 'action-button';
//                         deleteButton.onclick = function() {
//                             removeSchedule(index);
//                         };
//                         actionCell.appendChild(deleteButton);
//                         row.appendChild(actionCell);

//                         scheduleBody.appendChild(row);
//                     });
//                 })
//                 .catch(error => {
//                     console.error('Error loading schedules:', error);
//                     showStatus('schedule-status', 'Failed to load schedules', false);
//                 });
//         }

//         // Add new schedule
//         document.getElementById('add-schedule-button').addEventListener('click', function() {
//             const chamber = document.getElementById('chamber-select').value;
//             const hour = document.getElementById('hour-select').value;
//             const minute = document.getElementById('minute-select').value;

//             fetch('/add_schedule', {
//                 method: 'POST',
//                 headers: {
//                     'Content-Type': 'application/x-www-form-urlencoded',
//                 },
//                 body: `chamber=${chamber}&hour=${hour}&minute=${minute}`
//             })
//             .then(response => response.json())
//             .then(data => {
//                 if (data.status === 'success') {
//                     showStatus('schedule-status', 'Schedule added successfully!', true);
//                     loadSchedules(); // Reload schedules
//                 } else {
//                     showStatus('schedule-status', data.message || 'Failed to add schedule', false);
//                 }
//             })
//             .catch(error => {
//                 console.error('Error:', error);
//                 showStatus('schedule-status', 'An error occurred while adding schedule', false);
//             });
//         });

//         // Remove schedule
//         function removeSchedule(index) {
//             fetch('/remove_schedule', {
//                 method: 'POST',
//                 headers: {
//                     'Content-Type': 'application/x-www-form-urlencoded',
//                 },
//                 body: `index=${index}`
//             })
//             .then(response => response.json())
//             .then(data => {
//                 if (data.status === 'success') {
//                     showStatus('schedule-status', 'Schedule removed successfully!', true);
//                     loadSchedules(); // Reload schedules
//                 } else {
//                     showStatus('schedule-status', data.message || 'Failed to remove schedule', false);
//                 }
//             })
//             .catch(error => {
//                 console.error('Error:', error);
//                 showStatus('schedule-status', 'An error occurred while removing schedule', false);
//             });
//         }

//         // Helper function to show status messages
//         function showStatus(elementId, message, isSuccess) {
//             const statusElement = document.getElementById(elementId);
//             statusElement.textContent = message;
//             statusElement.className = 'status ' + (isSuccess ? 'success' : 'error');

//             // Hide status message after 3 seconds
//             setTimeout(() => {
//                 statusElement.style.display = 'none';
//             }, 3000);
//         }
//     </script>
// </body>
// </html>
// )rawliteral";

// #endif // HTML_ASSETS_H



#ifndef HTML_ASSETS_H
#define HTML_ASSETS_H

#include <Arduino.h>

// Login HTML page
const char LOGIN_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Medicine Reminder - Login</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f7ff;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }

        .card {
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
            padding: 30px;
            width: 350px;
            text-align: center;
        }

        h1 {
            color: #1a73e8;
            margin-bottom: 5px;
        }

        .subtitle {
            color: #5f6368;
            margin-bottom: 30px;
        }

        .form-group {
            margin-bottom: 15px;
            text-align: left;
        }

        label {
            display: block;
            margin-bottom: 5px;
            font-weight: 500;
            color: #5f6368;
        }

        input {
            width: 100%;
            padding: 10px;
            border: 1px solid #dadce0;
            border-radius: 4px;
            box-sizing: border-box;
            font-size: 14px;
        }

        .button {
            background-color: #1a73e8;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 12px;
            width: 100%;
            font-size: 14px;
            cursor: pointer;
            margin-top: 10px;
            font-weight: 500;
        }

        .note {
            margin-top: 15px;
            font-size: 12px;
            color: #5f6368;
        }

        .error {
            color: #d93025;
            margin-top: 10px;
            display: none;
        }

        .clock {
            margin-top: 10px;
            font-size: 12px;
            color: #5f6368;
        }
    </style>
</head>
<body>
    <div class="card">
        <h1>Smart Medicine Reminder</h1>
        <p class="subtitle">Health Monitoring System for Senior Citizens</p>

        <div class="form-group">
            <label for="username">Username</label>
            <input type="text" id="username" placeholder="Enter username" value="admin">
        </div>

        <div class="form-group">
            <label for="password">Password</label>
            <input type="password" id="password" placeholder="Enter password" value="admin">
        </div>

        <button class="button" id="login-button">Login</button>

        <div id="error-message" class="error">Invalid username or password</div>

        <p class="note">Default login: username "admin", password "admin"</p>

        <div class="clock">
            <div>Time: <span id="current-time">--:--:--</span></div>
            <div>Date: <span id="current-date">----/--/--</span></div>
        </div>
    </div>

    <script>
        // Update clock
        function updateClock() {
            fetch('/get_time')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('current-time').textContent = data.time;
                    document.getElementById('current-date').textContent = data.date;
                })
                .catch(error => {
                    console.error('Error fetching time:', error);
                });
        }

        // Update clock every second
        setInterval(updateClock, 1000);

        // Initial update
        updateClock();

        // Simple login authentication
        document.getElementById('login-button').addEventListener('click', function() {
            const username = document.getElementById('username').value;
            const password = document.getElementById('password').value;

            if (username === 'admin' && password === 'admin') {
                // Successful login
                window.location.href = '/dashboard';
            } else {
                // Failed login
                document.getElementById('error-message').style.display = 'block';
            }
        });
    </script>
</body>
</html>
)rawliteral";

// Dashboard HTML page with real-time clock
const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Medicine Reminder Dashboard</title>
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
        }

        h1 {
            color: #1a73e8;
        }

        .card {
            background-color: white;
            border-radius: 8px;
            box-shadow: 0 2px 6px rgba(0,0,0,0.1);
            padding: 20px;
            margin-bottom: 20px;
        }

        h2 {
            color: #202124;
            margin-top: 0;
        }

        .form-row {
            display: flex;
            margin-bottom: 15px;
            gap: 15px;
        }

        .form-group {
            flex: 1;
        }

        label {
            display: block;
            margin-bottom: 5px;
            font-weight: 500;
        }

        input, select {
            width: 100%;
            padding: 8px;
            border: 1px solid #dadce0;
            border-radius: 4px;
            box-sizing: border-box;
        }

        .button {
            background-color: #1a73e8;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 10px 15px;
            cursor: pointer;
            font-weight: 500;
        }

        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 15px;
        }

        th, td {
            border: 1px solid #e0e0e0;
            padding: 10px;
            text-align: left;
        }

        th {
            background-color: #f8f9fa;
        }

        .action-button {
            background-color: #f44336;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 5px 10px;
            cursor: pointer;
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

        .logout-button {
            background-color: #f1f3f4;
            color: #5f6368;
            border: none;
            border-radius: 4px;
            padding: 8px 15px;
            cursor: pointer;
            float: right;
            margin-top: 10px;
        }

        .clock-card {
            background-color: #1a73e8;
            color: white;
            border-radius: 8px;
            padding: 15px;
            margin-bottom: 20px;
            text-align: center;
        }

        .clock {
            font-size: 24px;
            font-weight: bold;
            margin-bottom: 5px;
        }

        .date {
            font-size: 16px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Medicine Reminder Dashboard</h1>
        <button class="logout-button" onclick="window.location.href='/'">Logout</button>

        <div class="clock-card">
            <div class="clock" id="current-time">--:--:--</div>
            <div class="date" id="current-date">----/--/--</div>
        </div>

        <div class="card">
            <h2>Patient Phone Number</h2>
            <div class="form-row">
                <div class="form-group">
                    <label for="patient-phone">Phone Number</label>
                    <input type="tel" id="patient-phone" placeholder="Enter patient's phone number">
                </div>
                <div class="form-group" style="flex: 0.3; display: flex; align-items: flex-end;">
                    <button class="button" id="save-phone-button">Save</button>
                </div>
            </div>
            <div id="phone-status" class="status"></div>
        </div>

        <div class="card">
            <h2>Add Medicine Schedule</h2>
            <div class="form-row">
                <div class="form-group">
                    <label for="chamber-select">Chamber</label>
                    <select id="chamber-select">
                        <option value="0">Chamber 1</option>
                        <option value="1">Chamber 2</option>
                        <option value="2">Chamber 3</option>
                        <option value="3">Chamber 4</option>
                        <option value="4">Chamber 5</option>
                        <option value="5">Chamber 6</option>
                        <option value="6">Chamber 7</option>
                        <option value="7">Chamber 8</option>
                    </select>
                </div>
                <div class="form-group">
                    <label for="hour-select">Hour</label>
                    <select id="hour-select">
                        <option value="0">00</option>
                        <option value="1">01</option>
                        <option value="2">02</option>
                        <option value="3">03</option>
                        <option value="4">04</option>
                        <option value="5">05</option>
                        <option value="6">06</option>
                        <option value="7">07</option>
                        <option value="8">08</option>
                        <option value="9">09</option>
                        <option value="10">10</option>
                        <option value="11">11</option>
                        <option value="12">12</option>
                        <option value="13">13</option>
                        <option value="14">14</option>
                        <option value="15">15</option>
                        <option value="16">16</option>
                        <option value="17">17</option>
                        <option value="18">18</option>
                        <option value="19">19</option>
                        <option value="20">20</option>
                        <option value="21">21</option>
                        <option value="22">22</option>
                        <option value="23">23</option>
                    </select>
                </div>
                <div class="form-group">
                    <label for="minute-select">Minute</label>
                    <select id="minute-select">
                        <option value="0">00</option>
                        <option value="5">05</option>
                        <option value="10">10</option>
                        <option value="15">15</option>
                        <option value="20">20</option>
                        <option value="25">25</option>
                        <option value="30">30</option>
                        <option value="35">35</option>
                        <option value="40">40</option>
                        <option value="45">45</option>
                        <option value="50">50</option>
                        <option value="55">55</option>
                    </select>
                </div>
                <div class="form-group" style="flex: 0.3; display: flex; align-items: flex-end;">
                    <button class="button" id="add-schedule-button">Add</button>
                </div>
            </div>
            <div id="schedule-status" class="status"></div>

            <h3>Current Schedules</h3>
            <table id="schedule-table">
                <thead>
                    <tr>
                        <th>Chamber</th>
                        <th>Time</th>
                        <th>Status</th>
                        <th>Action</th>
                    </tr>
                </thead>
                <tbody id="schedule-body">
                    <!-- Schedule items will be added here -->
                </tbody>
            </table>
        </div>
    </div>

    <script>
        // Update clock
        function updateClock() {
            fetch('/get_time')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('current-time').textContent = data.time;
                    document.getElementById('current-date').textContent = data.date;
                })
                .catch(error => {
                    console.error('Error fetching time:', error);
                });
        }

        // Update clock every second
        setInterval(updateClock, 1000);

        // Load data on page load
        document.addEventListener('DOMContentLoaded', function() {
            loadPhone();
            loadSchedules();
            updateClock(); // Initial clock update
        });

        // Load patient phone number
        function loadPhone() {
            fetch('/get_phone')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('patient-phone').value = data.phone;
                })
                .catch(error => {
                    console.error('Error loading phone:', error);
                    showStatus('phone-status', 'Failed to load phone number', false);
                });
        }

        // Save patient phone number
        document.getElementById('save-phone-button').addEventListener('click', function() {
            const phone = document.getElementById('patient-phone').value;

            fetch('/set_phone', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `phone=${phone}`
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    showStatus('phone-status', 'Phone number saved successfully!', true);
                } else {
                    showStatus('phone-status', 'Failed to save phone number', false);
                }
            })
            .catch(error => {
                console.error('Error:', error);
                showStatus('phone-status', 'An error occurred while saving phone number', false);
            });
        });

        // Load schedules
        function loadSchedules() {
            fetch('/get_schedules')
                .then(response => response.json())
                .then(data => {
                    const scheduleBody = document.getElementById('schedule-body');
                    scheduleBody.innerHTML = '';

                    if (!data.schedules || data.schedules.length === 0) {
                        // No schedules
                        const row = document.createElement('tr');
                        const cell = document.createElement('td');
                        cell.textContent = 'No schedules found';
                        cell.colSpan = 4;
                        cell.style.textAlign = 'center';
                        row.appendChild(cell);
                        scheduleBody.appendChild(row);
                        return;
                    }

                    data.schedules.forEach((schedule, index) => {
                        const row = document.createElement('tr');

                        // Chamber
                        const chamberCell = document.createElement('td');
                        chamberCell.textContent = `Chamber ${schedule.chamber + 1}`;
                        row.appendChild(chamberCell);

                        // Time
                        const timeCell = document.createElement('td');
                        const hour = schedule.hour.toString().padStart(2, '0');
                        const minute = schedule.minute.toString().padStart(2, '0');
                        timeCell.textContent = `${hour}:${minute}`;
                        row.appendChild(timeCell);

                        // Status
                        const statusCell = document.createElement('td');
                        statusCell.textContent = schedule.taken ? 'Taken' : 'Pending';
                        row.appendChild(statusCell);

                        // Action
                        const actionCell = document.createElement('td');
                        const deleteButton = document.createElement('button');
                        deleteButton.textContent = 'Delete';
                        deleteButton.className = 'action-button';
                        deleteButton.onclick = function() {
                            removeSchedule(index);
                        };
                        actionCell.appendChild(deleteButton);
                        row.appendChild(actionCell);

                        scheduleBody.appendChild(row);
                    });
                })
                .catch(error => {
                    console.error('Error loading schedules:', error);
                    showStatus('schedule-status', 'Failed to load schedules', false);
                });
        }

        // Add new schedule
        document.getElementById('add-schedule-button').addEventListener('click', function() {
            const chamber = document.getElementById('chamber-select').value;
            const hour = document.getElementById('hour-select').value;
            const minute = document.getElementById('minute-select').value;

            fetch('/add_schedule', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `chamber=${chamber}&hour=${hour}&minute=${minute}`
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    showStatus('schedule-status', 'Schedule added successfully!', true);
                    loadSchedules(); // Reload schedules
                } else {
                    showStatus('schedule-status', data.message || 'Failed to add schedule', false);
                }
            })
            .catch(error => {
                console.error('Error:', error);
                showStatus('schedule-status', 'An error occurred while adding schedule', false);
            });
        });

        // Remove schedule
        function removeSchedule(index) {
            fetch('/remove_schedule', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `index=${index}`
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    showStatus('schedule-status', 'Schedule removed successfully!', true);
                    loadSchedules(); // Reload schedules
                } else {
                    showStatus('schedule-status', data.message || 'Failed to remove schedule', false);
                }
            })
            .catch(error => {
                console.error('Error:', error);
                showStatus('schedule-status', 'An error occurred while removing schedule', false);
            });
        }

        // Helper function to show status messages
        function showStatus(elementId, message, isSuccess) {
            const statusElement = document.getElementById(elementId);
            statusElement.textContent = message;
            statusElement.className = 'status ' + (isSuccess ? 'success' : 'error');

            // Hide status message after 3 seconds
            setTimeout(() => {
                statusElement.style.display = 'none';
            }, 3000);
        }
    </script>
</body>
</html>
)rawliteral";

#endif // HTML_ASSETS_H