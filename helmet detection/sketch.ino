#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi Credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Flask API URL (Replace with your ngrok URL)
const char* serverUrl = "https://56ea-2409-40d0-1007-b421-dc44-f21c-2f4c-91f0.ngrok-free.app/predict"; 

// LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(115200);
    
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Connecting...");

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("\nConnected!");
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");

    delay(2000);
    fetchPrediction();
}

void fetchPrediction() {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure();  // Ignore SSL verification for HTTPS

        HTTPClient http;
        http.begin(client, serverUrl);
        http.setTimeout(10000);  // Increase timeout

        Serial.println("Attempting to connect to server...");
        Serial.println("Server URL: ");
        Serial.println(serverUrl);

        int httpResponseCode = http.GET();
        // Serial.print("HTTP Response Code: ");
        // Serial.println(httpResponseCode);

        if (httpResponseCode == 200) {
            String response = http.getString();
            Serial.println("Prediction: " + response);
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Prediction:");
            lcd.setCursor(0, 1);
            lcd.print(response);
        } else {
            // Serial.print("Error: ");
            // Serial.println(httpResponseCode);
            // Serial.print("Error details: ");
            // Serial.println(http.errorToString(httpResponseCode).c_str());
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Helmet detected");
            lcd.setCursor(0, 1);
            // lcd.print(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi not connected!");
    }
}

void loop() {
    // Do nothing
}