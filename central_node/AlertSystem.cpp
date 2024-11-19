#include "AlertSystem.h"

AlertSystem::AlertSystem() {}

void AlertSystem::begin() {
    pinMode(ALERT_LED, OUTPUT);
    pinMode(ALERT_BUZZER, OUTPUT);
}

void AlertSystem::checkAlerts(float temp, float hum, uint16_t soil, 
                            uint16_t gas, uint16_t flame, bool water) {
    bool alertCondition = false;
    String alertMessage = "";
    
    // Temperature alert
    if (temp > thresholds.temp_high) {
        alertMessage += "HIGH TEMPERATURE! ";
        alertCondition = true;
    }
    
    // Humidity alert
    if (hum > thresholds.hum_high) {
        alertMessage += "HIGH HUMIDITY! ";
        alertCondition = true;
    }
    
    // Soil moisture alert
    if (soil > thresholds.soil_wet) {
        alertMessage += "SOIL TOO WET! ";
        alertCondition = true;
    }
    
    // Gas alert
    if (gas > thresholds.gas_high) {
        alertMessage += "GAS DETECTED! ";
        alertCondition = true;
    }
    
    // Flame alert
    if (flame < thresholds.flame_threshold) {
        alertMessage += "FLAME DETECTED! ";
        alertCondition = true;
    }
    
    // Water alert
    if (water) {
        alertMessage += "WATER DETECTED! ";
        alertCondition = true;
    }
    
    if (alertCondition) {
        triggerAlert(alertMessage);
    }
}

void AlertSystem::triggerAlert(String message) {
    digitalWrite(ALERT_LED, HIGH);
    digitalWrite(ALERT_BUZZER, HIGH);
    Serial.println("ALERT: " + message);
    delay(1000);
    digitalWrite(ALERT_BUZZER, LOW);
    // LED stays on until next reading cycle
}