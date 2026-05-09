# IoT Based Smart Plant Monitoring and Alert System using ESP8266 and Render Cloud

## Project Overview
This project implements a complete IoT-based smart plant monitoring system that tracks soil moisture, temperature, and humidity to ensure optimal plant health. The system uses an ESP8266 NodeMCU to collect sensor data, displays it on an LCD, triggers alerts via a buzzer, and sends data to a cloud dashboard hosted on Render.

## Components Used
1. ESP8266 NodeMCU
2. Soil Moisture Sensor (YL-69 with LM393 module)
3. DHT11 Temperature and Humidity Sensor
4. 16x2 LCD Display (I2C)
5. Buzzer
6. Jumper Wires
7. Breadboard

## Circuit Diagram
### ESP8266 NodeMCU Pin Connections:

**Soil Moisture Sensor:**
- VCC → 3V3
- GND → GND
- AO → A0

**DHT11 Sensor:**
- VCC → 3V3
- GND → GND
- DATA → D4

**Buzzer:**
- Positive → D5
- Negative → GND

**LCD Display (I2C):**
- SDA → D2
- SCL → D1
- VCC → VIN
- GND → GND

### Circuit Explanation:
1. The soil moisture sensor's analog output is connected to A0 for reading moisture levels.
2. DHT11 data pin is connected to D4 for temperature and humidity readings.
3. Buzzer is connected to D5 and will beep when alerts are triggered.
4. LCD uses I2C communication with SDA on D2 and SCL on D1 for displaying sensor data.
5. All sensors share the 3.3V power supply from the ESP8266.

## Folder Structure
```
SmartPlantMonitoring/
├── esp8266_code/
│   └── SmartPlantMonitoring.ino
├── backend/
│   ├── package.json
│   └── server.js
├── frontend/
│   ├── index.html
│   ├── styles.css
│   └── script.js
├── public/
│   ├── index.html
│   ├── styles.css
│   └── script.js
├── package.json
├── server.js
└── README.md
```

## Required Libraries
### For ESP8266 (Arduino IDE):
1. ESP8266WiFi
2. ESP8266HTTPClient
3. DHT sensor library by Adafruit
4. LiquidCrystal_I2C

### For Backend:
- express
- cors
- body-parser

## Setup Instructions

### 1. ESP8266 Setup
1. Install Arduino IDE
2. Add ESP8266 board support: File → Preferences → Additional Boards Manager URLs: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
3. Install ESP8266 board in Board Manager
4. Install required libraries via Library Manager
5. Update WiFi credentials and server URL in the code
6. Upload the code to ESP8266

### 2. Backend Deployment on Render
1. Create a free account on Render.com
2. Push the entire repository to GitHub
3. Connect your GitHub repository to Render
4. Create a new **Web Service**
5. Select the repository and branch
6. Configure the service:
   - **Runtime**: Node (auto-detected)
   - **Build Command**: `npm install`
   - **Start Command**: `npm start`
7. Deploy the service
8. Note the service URL (e.g., https://your-app.onrender.com)
9. Update the `serverName` in `esp8266_code/SmartPlantMonitoring.ino` with your Render URL
10. Access the dashboard at your Render URL

## API Endpoints
- `GET /`: Serves the dashboard
- `POST /update`: Receives sensor data from ESP8266
- `GET /data`: Returns latest sensor data for the dashboard

## API Endpoints
- `POST /update`: Receives sensor data from ESP8266
- `GET /data`: Returns latest sensor data for the dashboard

## Working Logic
1. ESP8266 reads sensor data every 10 seconds
2. Data is displayed on the LCD in alternating screens
3. Buzzer activates for alerts (dry soil or high temperature)
4. Data is sent to the Render backend via HTTP POST
5. Frontend fetches data every 5 seconds and updates the dashboard
6. Dashboard shows real-time values and plant health status

## Thresholds and Alerts
- Soil Dry Threshold: > 300 (adjust based on your sensor)
- Temperature High Threshold: > 30°C
- Alerts trigger buzzer and update health status

## Error Handling
- DHT sensor read failures are logged to Serial
- WiFi disconnection is detected and logged
- HTTP request failures are logged
- Frontend handles fetch errors gracefully

## Debugging Tips
1. Use Serial Monitor to check sensor readings and WiFi connection
2. Verify I2C address for LCD (may be 0x3F instead of 0x27)
3. Calibrate soil moisture sensor in air and water to determine thresholds
4. Check firewall settings if ESP8266 can't connect to server
5. Use browser developer tools to debug frontend issues

## Optimization Features
- Lightweight code with minimal memory usage
- Efficient sensor reading with error checking
- Stable WiFi reconnection handling
- Auto-refreshing dashboard
- Mobile-responsive design
- Low power consumption through timed operations

## Optional Advanced Features
- Auto water pump integration (add relay module)
- Email alerts using services like SendGrid
- Telegram bot notifications
- Historical data graphs with Chart.js
- Database integration (MongoDB/Firebase)
- Multiple plant monitoring with sensor arrays

## Power Consumption
- ESP8266 operates at 3.3V
- Sensors draw minimal current
- Buzzer only activates during alerts
- Deep sleep mode can be implemented for battery operation

## Security Considerations
- Use HTTPS for production deployment
- Implement API key authentication if needed
- Avoid exposing sensitive WiFi credentials in code

## Troubleshooting Deployment

### Common Render Deployment Issues
1. **Build Fails**: Check Render logs for npm install errors. Ensure all dependencies are listed in package.json.
2. **Start Fails**: Verify server.js has no syntax errors. Check that PORT environment variable is used.
3. **Dashboard Not Loading**: Ensure public/ folder is committed and contains index.html, styles.css, script.js.
4. **API Not Working**: Test /data endpoint returns JSON. Check CORS settings.
5. **ESP8266 Can't Connect**: Verify server URL in Arduino code matches Render URL exactly.

### Debugging Steps
1. Check Render service logs in the dashboard
2. Test locally: `npm install && npm start` (requires Node.js installed)
3. Verify all files are committed to GitHub
4. Ensure package.json specifies correct Node version (18.x)
5. Check that server listens on process.env.PORT

### If Still Failing
- Try redeploying after pushing changes
- Clear build cache in Render settings
- Check GitHub repository has all required files in root

This system provides a complete, production-ready IoT solution for plant monitoring with real-time alerts and cloud-based data visualization.