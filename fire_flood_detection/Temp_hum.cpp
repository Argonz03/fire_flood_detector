#include "DigitalSensors.h"
#include <Arduino.h>
#include <DHT.h>



Temp_Hum::Temp_Hum(int pin) : dht22(pin, DHT22) { // Initialize with pin and type
    dht22.begin(); // Start the sensor
}

void Temp_Hum::takeReading() { // Debugging function
    float t = dht22.readTemperature(false); // Get temperature
    float h = dht22.readHumidity(); // Get humidity

    if (isnan(t) || isnan(h)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Hum: "); Serial.print(h, 1); Serial.print("\t");
    Serial.print("Temp: "); Serial.println(t, 1);
}

float Temp_Hum::temperature() {
    return dht22.readTemperature(false); // Get temperature
}

float Temp_Hum::humidity() {
    return dht22.readHumidity(); // Get humidity
}