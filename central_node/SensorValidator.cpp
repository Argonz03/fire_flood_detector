#include "SensorValidator.h"

SensorValidator::SensorValidator() {
    // Initialize error counts
    for(int i = 0; i < 6; i++) {
        errorCounts[i] = 0;
    }
}

bool SensorValidator::validateReadings(float temp, float hum, uint16_t soil, 
                                     uint16_t gas, uint16_t flame) {
    bool isValid = true;
    
    // Temperature check
    if (temp < ranges.temp_min || temp > ranges.temp_max) {
        errorCounts[0]++;
        isValid = false;
        Serial.println("Temperature reading out of range");
    } else {
        errorCounts[0] = 0;
    }
    
    // Humidity check
    if (hum < ranges.hum_min || hum > ranges.hum_max) {
        errorCounts[1]++;
        isValid = false;
        Serial.println("Humidity reading out of range");
    } else {
        errorCounts[1] = 0;
    }
    
    // Soil moisture check
    if (soil < ranges.soil_min || soil > ranges.soil_max) {
        errorCounts[2]++;
        isValid = false;
        Serial.println("Soil moisture reading out of range");
    } else {
        errorCounts[2] = 0;
    }
    
    // Gas sensor check
    if (gas < ranges.gas_min || gas > ranges.gas_max) {
        errorCounts[3]++;
        isValid = false;
        Serial.println("Gas reading out of range");
    } else {
        errorCounts[3] = 0;
    }
    
    // Flame sensor check
    if (flame < ranges.flame_min || flame > ranges.flame_max) {
        errorCounts[4]++;
        isValid = false;
        Serial.println("Flame reading out of range");
    } else {
        errorCounts[4] = 0;
    }
    
    // Check for consecutive errors
    for (int i = 0; i < 6; i++) {
        if (errorCounts[i] >= ERROR_THRESHOLD) {
            Serial.printf("Sensor %d has failed %d consecutive readings\n", 
                        i, ERROR_THRESHOLD);
        }
    }
    
    return isValid;
}