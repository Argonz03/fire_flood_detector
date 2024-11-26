#include "AlertSystem.h"

AlertSystem::AlertSystem() {}

void AlertSystem::checkAlerts(float temp, float objtemp, uint16_t soil, 
                            uint16_t gas, uint16_t flame, bool water) {
    bool alertCondition = false;
    String alertMessage = "";

    int tempStatus = 0, objTempStatus = 0, soilStatus = 0;
    int gasStatus = 0, flameStatus = 0, waterStatus = 0;

    
    // Temperature alert
    if (temp > thresholds.temp_high) {
        alertMessage += "HIGH TEMPERATURE! ";
        alertCondition = true;
        tempStatus = 1; // Elevated
    }
    
    // Directional Temp
    if (objtemp > temp + 15) {
        alertMessage += "DIRECTIONAL TEMPERATURE HOTTER THAN ENVIROMENT ";
        alertCondition = true;
        objTempStatus = 1; // Elevated
    }
    
    // Soil moisture alert
    if (soil > thresholds.soil_wet) {
        alertMessage += "SOIL TOO WET! ";
        alertCondition = true;
        soilStatus = 1; // Elevated
    }
    
    // Gas alert
    if (gas > thresholds.gas_high) {
        alertMessage += "GAS DETECTED! ";
        alertCondition = true;
        gasStatus = 1; //Elevated
    }
    
    // Flame alert
    if (flame < thresholds.flame_threshold) {
        alertMessage += "FLAME DETECTED! ";
        alertCondition = true;
        flameStatus = 2; // Danger
    }
    
    // Water alert
    if (water) {
        alertMessage += "WATER DETECTED! ";
        alertCondition = true;
        waterStatus = 1; //Danger
    }


    if (water && soil > thresholds.soil_wet) {
        alertMessage += "WATER DETECTED! ";
        alertCondition = true;
        waterStatus = 2; //Danger
    }

     // Consolidate Flood Alert
    floodAlert = max(soilStatus, waterStatus);

    // Consolidate Fire Alert
    fireAlert = max(max(tempStatus, objTempStatus), max(gasStatus, flameStatus));

    // Overall Alert Status
    alertStatus = max(floodAlert, fireAlert);

    if (alertCondition) {
        triggerAlert(alertMessage);
    }    

    
}

void AlertSystem::triggerAlert(String message) {
    Serial.println("ALERT: " + message);

    Serial.println("Flood Alert: " + String(floodAlert));
    Serial.println("Fire Alert: " + String(fireAlert));
    Serial.println("Overall Alert Status: " + String(alertStatus));

}

uint8_t AlertSystem::getFloodAlert() const {
    return floodAlert;
}

uint8_t AlertSystem::getFireAlert() const {
    return fireAlert;
}

uint8_t AlertSystem::getAlertStatus() const {
    return alertStatus;
}