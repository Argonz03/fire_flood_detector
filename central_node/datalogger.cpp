#include "DataLogger.h"
#include <Arduino.h>

// Constructor
DataLogger::DataLogger() {}

// Initialize Firebase connection
void DataLogger::begin(const char* firebaseHost, const char* firebaseAuth) {
    // Set the Firebase project API key
    config.api_key = "";  // Firebase API Key
    
    // Set the RTDB URL
    config.database_url = "";  // Firebase Database URL 
    
    // Initialize Firebase with the provided configuration
    Firebase.begin(&config, &auth);
}

// Log sensor data to Firebase
void DataLogger::logData(float temp, float hum, uint16_t soil, uint16_t gas, 
                        uint16_t flame, bool water, double objTemp, 
                        double ambTemp, int16_t rssi) {
    // Check if Firebase connection is ready
    if (Firebase.ready()) {
        // Create a JSON object to store the data
        FirebaseJson json;
        
        // Add all sensor readings to the JSON object
        json.set("timestamp", String(millis()));  // Use current time as timestamp
        json.set("temperature", temp);
        json.set("humidity", hum);
        json.set("soilMoisture", soil);
        json.set("gas", gas);
        json.set("flame", flame);
        json.set("waterDetected", water);
        json.set("objectTemp", objTemp);
        json.set("ambientTemp", ambTemp);
        json.set("rssi", rssi);

        // Create a path with a timestamp
        String path = "/sensor_logs/" + String(millis());
        
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