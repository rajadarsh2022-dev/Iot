#include &lt;ESP8266WiFi.h&gt;
#include &lt;ESP8266HTTPClient.h&gt;
#include &lt;WiFiClient.h&gt;
#include &lt;DHT.h&gt;
#include &lt;LiquidCrystal_I2C.h&gt;
#include &lt;Wire.h&gt;

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Server details
const char* serverName = "https://your-render-app.onrender.com/update"; // Replace with your Render URL

// Pin definitions
#define SOIL_MOISTURE_PIN A0
#define DHT_PIN D4
#define BUZZER_PIN D5
#define DHT_TYPE DHT11

// Thresholds
const int SOIL_DRY_THRESHOLD = 300; // Adjust based on sensor
const float TEMP_HIGH_THRESHOLD = 30.0; // Celsius

// Sensors
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows

// Variables
int soilMoisture = 0;
float temperature = 0.0;
float humidity = 0.0;
bool buzzerActive = false;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize LCD
  Wire.begin(D2, D1); // SDA, SCL
  lcd.init();
  lcd.backlight();

  // Initialize DHT
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");

  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("Smart Plant");
  lcd.setCursor(0, 1);
  lcd.print("Monitor System");
  delay(2000);
}

void loop() {
  // Read sensors
  soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Check for sensor errors
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Determine plant health
  String healthStatus = "Healthy";
  bool waterNeeded = soilMoisture &gt; SOIL_DRY_THRESHOLD;
  bool tempAlert = temperature &gt; TEMP_HIGH_THRESHOLD;

  if (waterNeeded) healthStatus = "Water Needed";
  if (tempAlert) healthStatus = "High Temp Alert";

  // Activate buzzer if needed
  if (waterNeeded || tempAlert) {
    if (!buzzerActive) {
      digitalWrite(BUZZER_PIN, HIGH);
      buzzerActive = true;
    }
  } else {
    if (buzzerActive) {
      digitalWrite(BUZZER_PIN, LOW);
      buzzerActive = false;
    }
  }

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%");

  delay(2000); // Show temp/hum for 2s

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil: ");
  lcd.print(soilMoisture);
  lcd.setCursor(0, 1);
  lcd.print(healthStatus);

  // Send data to server
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{";
    jsonData += "\"temperature\":" + String(temperature) + ",";
    jsonData += "\"humidity\":" + String(humidity) + ",";
    jsonData += "\"soilMoisture\":" + String(soilMoisture) + ",";
    jsonData += "\"healthStatus\":\"" + healthStatus + "\"";
    jsonData += "}";

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode &gt; 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(10000); // Send data every 10 seconds
}