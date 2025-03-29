// #ifndef HTML_ASSETS_H
// #define HTML_ASSETS_H

// #include <Arduino.h>

// // Login HTML page
// const char LOGIN_HTML[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html lang="en">
// <head>
//     <meta charset="UTF-8">
//     <meta name="viewport" content="width=device-width, initial-scale=1.0">
//     <title>Smart Medicine Reminder - Login</title>
//     <style>
//         body {
//             font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
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

//         .tab-container {
//             display: flex;
//             border-bottom: 1px solid #dadce0;
//             margin-bottom: 20px;
//         }

//         .tab {
//             flex: 1;
//             padding: 10px;
//             cursor: pointer;
//             position: relative;
//         }

//         .tab.active {
//             color: #1a73e8;
//             font-weight: 500;
//         }

//         .tab.active::after {
//             content: '';
//             position: absolute;
//             bottom: -1px;
//             left: 0;
//             width: 100%;
//             height: 2px;
//             background-color: #1a73e8;
//         }

//         .form-group {
//             margin-bottom: 15px;
//             text-align: left;
//         }

//         label {
//             display: block;
//             margin-bottom: 5px;
//             color: #5f6368;
//             font-weight: 500;
//         }

//         input, select {
//             width: 100%;
//             padding: 10px;
//             border: 1px solid #dadce0;
//             border-radius: 4px;
//             box-sizing: border-box;
//             font-size: 14px;
//         }

//         input:focus, select:focus {
//             outline: none;
//             border-color: #1a73e8;
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

//         .button:hover {
//             background-color: #0d66d0;
//         }

//         .form-container {
//             display: none;
//         }

//         .form-container.active {
//             display: block;
//         }
//     </style>
// </head>
// <body>
//     <div class="card">
//         <h1>Smart Medicine Reminder</h1>
//         <p class="subtitle">Health Monitoring System for Senior Citizens</p>

//         <div class="tab-container">
//             <div class="tab active" id="login-tab">Login</div>
//             <div class="tab" id="signup-tab">Sign Up</div>
//         </div>

//         <!-- Login Form -->
//         <div class="form-container active" id="login-form">
//             <div class="form-group">
//                 <label for="username">Username</label>
//                 <input type="text" id="username" placeholder="Enter username">
//             </div>

//             <div class="form-group">
//                 <label for="password">Password</label>
//                 <input type="password" id="password" placeholder="Enter password">
//             </div>

//             <div class="form-group">
//                 <label for="role">Role</label>
//                 <select id="role">
//                     <option value="" selected disabled>Select role</option>
//                     <option value="patient">Patient</option>
//                     <option value="caregiver">Caregiver</option>
//                     <option value="doctor">Doctor</option>
//                 </select>
//             </div>

//             <button class="button" id="login-button">Login</button>
//         </div>

//         <!-- Signup Form -->
//         <div class="form-container" id="signup-form">
//             <div class="form-group">
//                 <label for="new-username">Username</label>
//                 <input type="text" id="new-username" placeholder="Choose a username">
//             </div>

//             <div class="form-group">
//                 <label for="new-password">Password</label>
//                 <input type="password" id="new-password" placeholder="Choose a password">
//             </div>

//             <div class="form-group">
//                 <label for="confirm-password">Confirm Password</label>
//                 <input type="password" id="confirm-password" placeholder="Confirm password">
//             </div>

//             <div class="form-group">
//                 <label for="new-role">Role</label>
//                 <select id="new-role">
//                     <option value="" selected disabled>Select role</option>
//                     <option value="patient">Patient</option>
//                     <option value="caregiver">Caregiver</option>
//                     <option value="doctor">Doctor</option>
//                 </select>
//             </div>

//             <div class="form-group">
//                 <label for="phone">Phone Number</label>
//                 <input type="tel" id="phone" placeholder="Enter phone number">
//             </div>

//             <button class="button" id="signup-button">Sign Up</button>
//         </div>
//     </div>

//     <script>
//         // Tab switching logic
//         const loginTab = document.getElementById('login-tab');
//         const signupTab = document.getElementById('signup-tab');
//         const loginForm = document.getElementById('login-form');
//         const signupForm = document.getElementById('signup-form');

//         loginTab.addEventListener('click', function() {
//             loginTab.classList.add('active');
//             signupTab.classList.remove('active');
//             loginForm.classList.add('active');
//             signupForm.classList.remove('active');
//         });

//         signupTab.addEventListener('click', function() {
//             signupTab.classList.add('active');
//             loginTab.classList.remove('active');
//             signupForm.classList.add('active');
//             loginForm.classList.remove('active');
//         });

//         // Login form handling
//         document.getElementById('login-button').addEventListener('click', function() {
//             const username = document.getElementById('username').value;
//             const password = document.getElementById('password').value;
//             const role = document.getElementById('role').value;

//             if (!username || !password || !role) {
//                 alert('Please fill in all fields');
//                 return;
//             }

//             fetch('/login', {
//                 method: 'POST',
//                 headers: {
//                     'Content-Type': 'application/x-www-form-urlencoded',
//                 },
//                 body: `username=${username}&password=${password}&role=${role}`
//             })
//             .then(response => response.json())
//             .then(data => {
//                 if (data.status === 'success') {
//                     window.location.href = '/dashboard';
//                 } else {
//                     alert(data.message);
//                 }
//             })
//             .catch(error => {
//                 console.error('Error:', error);
//                 alert('An error occurred during login');
//             });
//         });

//         // Signup form handling
//         document.getElementById('signup-button').addEventListener('click', function() {
//             const username = document.getElementById('new-username').value;
//             const password = document.getElementById('new-password').value;
//             const confirmPassword = document.getElementById('confirm-password').value;
//             const role = document.getElementById('new-role').value;
//             const phone = document.getElementById('phone').value;

//             if (!username || !password || !confirmPassword || !role || !phone) {
//                 alert('Please fill in all fields');
//                 return;
//             }

//             if (password !== confirmPassword) {
//                 alert('Passwords do not match');
//                 return;
//             }

//             fetch('/signup', {
//                 method: 'POST',
//                 headers: {
//                     'Content-Type': 'application/x-www-form-urlencoded',
//                 },
//                 body: `username=${username}&password=${password}&role=${role}&phone=${phone}`
//             })
//             .then(response => response.json())
//             .then(data => {
//                 alert(data.message);
//                 if (data.status === 'success') {
//                     loginTab.click();
//                 }
//             })
//             .catch(error => {
//                 console.error('Error:', error);
//                 alert('An error occurred during signup');
//             });
//         });
//     </script>
// </body>
// </html>
// )rawliteral";

// // Dashboard HTML page
// const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html lang="en">
// <head>
//     <meta charset="UTF-8">
//     <meta name="viewport" content="width=device-width, initial-scale=1.0">
//     <title>Medicine Reminder Dashboard</title>
//     <style>
//         body {
//             font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
//             background-color: #f5f5f5;
//             margin: 0;
//             padding: 20px;
//         }

//         .container {
//             max-width: 1000px;
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

//         .button:hover {
//             background-color: #0d66d0;
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
//     </style>
// </head>
// <body>
//     <div class="container">
//         <h1>Medicine Reminder Dashboard</h1>

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
//                     <label for="time-input">Time</label>
//                     <input type="time" id="time-input">
//                 </div>
//                 <div class="form-group" style="flex: 0.3; display: flex; align-items: flex-end;">
//                     <button class="button" id="add-schedule-button">Add</button>
//                 </div>
//             </div>

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
//             fetch('/getphone')
//                 .then(response => response.json())
//                 .then(data => {
//                     document.getElementById('patient-phone').value = data.phone;
//                 })
//                 .catch(error => {
//                     console.error('Error loading phone:', error);
//                 });
//         }

//         // Save patient phone number
//         document.getElementById('save-phone-button').addEventListener('click', function() {
//             const phone = document.getElementById('patient-phone').value;

//             fetch('/phone', {
//                 method: 'POST',
//                 headers: {
//                     'Content-Type': 'application/x-www-form-urlencoded',
//                 },
//                 body: `phone=${phone}`
//             })
//             .then(response => response.json())
//             .then(data => {
//                 if (data.status === 'success') {
//                     alert('Phone number saved successfully!');
//                 } else {
//                     alert('Failed to save phone number');
//                 }
//             })
//             .catch(error => {
//                 console.error('Error:', error);
//                 alert('An error occurred while saving phone number');
//             });
//         });

//         // Load schedules
//         function loadSchedules() {
//             fetch('/getschedule')
//                 .then(response => response.json())
//                 .then(data => {
//                     const scheduleBody = document.getElementById('schedule-body');
//                     scheduleBody.innerHTML = '';

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
//                 });
//         }

//         // Add new schedule
//         document.getElementById('add-schedule-button').addEventListener('click', function() {
//             const chamber = document.getElementById('chamber-select').value;
//             const timeInput = document.getElementById('time-input').value;

//             if (!timeInput) {
//                 alert('Please select a time');
//                 return;
//             }

//             const [hours, minutes] = timeInput.split(':');

//             // Get current schedules first
//             fetch('/getschedule')
//                 .then(response => response.json())
//                 .then(data => {
//                     const schedules = data.schedules || [];

//                     // Add new schedule
//                     schedules.push({
//                         chamber: parseInt(chamber),
//                         hour: parseInt(hours),
//                         minute: parseInt(minutes),
//                         taken: false
//                     });

//                     // Update schedules
//                     updateSchedules(schedules);
//                 })
//                 .catch(error => {
//                     console.error('Error:', error);
//                 });
//         });

//         // Remove schedule
//         function removeSchedule(index) {
//             fetch('/getschedule')
//                 .then(response => response.json())
//                 .then(data => {
//                     const schedules = data.schedules || [];
//                     schedules.splice(index, 1);
//                     updateSchedules(schedules);
//                 })
//                 .catch(error => {
//                     console.error('Error:', error);
//                 });
//         }

//         // Update schedules on server
//         function updateSchedules(schedules) {
//             const scheduleJson = JSON.stringify({ schedules: schedules });

//             fetch('/schedule', {
//                 method: 'POST',
//                 headers: {
//                     'Content-Type': 'application/x-www-form-urlencoded',
//                 },
//                 body: `schedule=${encodeURIComponent(scheduleJson)}`
//             })
//             .then(response => response.json())
//             .then(data => {
//                 if (data.status === 'success') {
//                     loadSchedules(); // Reload schedules
//                 } else {
//                     alert('Failed to update schedules');
//                 }
//             })
//             .catch(error => {
//                 console.error('Error:', error);
//                 alert('An error occurred while updating schedules');
//             });
//         }
//     </script>
// </body>
// </html>
// )rawliteral";

// #endif // HTML_ASSETS_H



#ifndef HTML_ASSETS_H
#define HTML_ASSETS_H

#include <Arduino.h>

// Login HTML page - Simplified
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
            color: #5f6368;
            font-weight: 500;
        }

        input, select {
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
            margin-top: 20px;
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
            <input type="text" id="username" placeholder="Enter username">
        </div>

        <div class="form-group">
            <label for="password">Password</label>
            <input type="password" id="password" placeholder="Enter password">
        </div>

        <div class="form-group">
            <label for="role">Role</label>
            <select id="role">
                <option value="admin">Admin</option>
                <option value="patient">Patient</option>
                <option value="caregiver">Caregiver</option>
            </select>
        </div>

        <button class="button" id="login-button">Login</button>

        <p class="note">Default login: username "admin", password "admin"</p>
    </div>

    <script>
        // Login form handling
        document.getElementById('login-button').addEventListener('click', function() {
            const username = document.getElementById('username').value;
            const password = document.getElementById('password').value;
            const role = document.getElementById('role').value;

            if (!username || !password) {
                alert('Please enter username and password');
                return;
            }

            fetch('/login', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `username=${username}&password=${password}&role=${role}`
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    window.location.href = '/dashboard';
                } else {
                    alert(data.message);
                }
            })
            .catch(error => {
                console.error('Error:', error);
                alert('An error occurred during login');
            });
        });
    </script>
</body>
</html>
)rawliteral";

// Dashboard HTML page - Simplified
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
    </style>
</head>
<body>
    <div class="container">
        <h1>Medicine Reminder Dashboard</h1>

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
                    <label for="time-input">Time</label>
                    <input type="time" id="time-input">
                </div>
                <div class="form-group" style="flex: 0.3; display: flex; align-items: flex-end;">
                    <button class="button" id="add-schedule-button">Add</button>
                </div>
            </div>

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
        // Load data on page load
        document.addEventListener('DOMContentLoaded', function() {
            loadPhone();
            loadSchedules();
        });

        // Load patient phone number
        function loadPhone() {
            fetch('/getphone')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('patient-phone').value = data.phone;
                })
                .catch(error => {
                    console.error('Error loading phone:', error);
                });
        }

        // Save patient phone number
        document.getElementById('save-phone-button').addEventListener('click', function() {
            const phone = document.getElementById('patient-phone').value;

            fetch('/phone', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `phone=${phone}`
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    alert('Phone number saved successfully!');
                } else {
                    alert('Failed to save phone number');
                }
            })
            .catch(error => {
                console.error('Error:', error);
                alert('An error occurred while saving phone number');
            });
        });

        // Load schedules
        function loadSchedules() {
            fetch('/getschedule')
                .then(response => response.json())
                .then(data => {
                    const scheduleBody = document.getElementById('schedule-body');
                    scheduleBody.innerHTML = '';

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
                });
        }

        // Add new schedule
        document.getElementById('add-schedule-button').addEventListener('click', function() {
            const chamber = document.getElementById('chamber-select').value;
            const timeInput = document.getElementById('time-input').value;

            if (!timeInput) {
                alert('Please select a time');
                return;
            }

            const [hours, minutes] = timeInput.split(':');

            // Get current schedules first
            fetch('/getschedule')
                .then(response => response.json())
                .then(data => {
                    const schedules = data.schedules || [];

                    // Add new schedule
                    schedules.push({
                        chamber: parseInt(chamber),
                        hour: parseInt(hours),
                        minute: parseInt(minutes),
                        taken: false
                    });

                    // Update schedules
                    updateSchedules(schedules);
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        });

        // Remove schedule
        function removeSchedule(index) {
            fetch('/getschedule')
                .then(response => response.json())
                .then(data => {
                    const schedules = data.schedules || [];
                    schedules.splice(index, 1);
                    updateSchedules(schedules);
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Update schedules on server
        function updateSchedules(schedules) {
            const scheduleJson = JSON.stringify({ schedules: schedules });

            fetch('/schedule', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `schedule=${encodeURIComponent(scheduleJson)}`
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    loadSchedules(); // Reload schedules
                } else {
                    alert('Failed to update schedules');
                }
            })
            .catch(error => {
                console.error('Error:', error);
                alert('An error occurred while updating schedules');
            });
        }
    </script>
</body>
</html>
)rawliteral";

#endif // HTML_ASSETS_H