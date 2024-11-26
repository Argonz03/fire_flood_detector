#include "DataLogger.h"
#include <Arduino.h>

// Constructor
DataLogger::DataLogger() {}

// Initialize Firebase connection
void DataLogger::begin() {

    bool signupOK = false;

    // Set the Firebase project API key
    config.api_key = "AIzaSyDTfi2kJ20UMK2PQDzoWVDilMqYRiAmphQ";  // Firebase API Key
    
    // Set the RTDB URL
    config.database_url = "https://guardly-695d8-default-rtdb.firebaseio.com";  // Firebase Database URL 

    // Firebase anonymous sign-up
    if (Firebase.signUp(&config, &auth, "", "")) {
        Serial.println("Firebase sign-up successful!");
        signupOK = true;
    } else {
        Serial.print("Firebase sign-up error: ");
        Serial.println(config.signer.signupError.message.c_str());
    }
    
    // Initialize Firebase with the provided configuration
    Firebase.begin(&config, &auth);

    configTime(0, 0, "time.google.com", "time.nist.gov"); // UTC timezone
    struct tm timeInfo;
      if (!getLocalTime(&timeInfo)) {
        Serial.println("Failed to obtain time");
    } else {
        Serial.println("Time synchronized successfully");
    }


}

DataLogger::DateTime DataLogger::getCurrentDateTime() {
    DateTime dateTime;

    struct tm timeInfo;
    if (!getLocalTime(&timeInfo)) {
        Serial.println("Failed to get local time");
        dateTime.fullDate = "unknown";
        dateTime.fullTime = "unknown";
        return dateTime;
    }

    // Extract full date
    char dateBuffer[11];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &timeInfo);
    dateTime.fullDate = String(dateBuffer);

    // Extract full time
    char timeBuffer[9];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeInfo);
    dateTime.fullTime = String(timeBuffer);


    return dateTime;
}

// Log sensor data to Firebase
void DataLogger::logData(float temp, float hum, uint16_t soil, uint16_t gas, 
                        uint16_t flame, bool water, double objTemp, 
                        double ambTemp, int16_t rssi, uint16_t batteryReading) {


    DateTime currentDateTime = getCurrentDateTime();



    // Check if Firebase connection is ready
    if (Firebase.ready()) {
      Serial.println("Fire base Ready");
        // Create a JSON object to store the data
        FirebaseJson json;
  
        
        // Add all sensor readings to the JSON object

        json.set("temperature", temp);
        json.set("humidity", hum);
        json.set("soilMoisture", soil);
        json.set("gas", gas);
        json.set("flame", flame);
        json.set("waterDetected", water);
        json.set("objectTemp", objTemp);
        json.set("ambientTemp", ambTemp);
        json.set("rssi", rssi);
        json.set("Battery Percentage", batteryReading);
    

        // Create a path with a timestamp
        String path = "/sensor_logs/" + currentDateTime.fullDate + "/" + currentDateTime.fullTime;

        
        // Push the JSON object to Firebase
        if (Firebase.RTDB.pushJSON(&fbdo, path.c_str(), &json)) {
            Serial.println("Data logged to Firebase successfully");
        } else {
            // If push failed, print error message
            Serial.println("Failed to log data to Firebase");
            Serial.println("Reason: " + fbdo.errorReason());
        }
    } else {
        Serial.println("Firebase not ready");
    }
}

