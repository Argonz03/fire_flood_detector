#ifndef ALERTSYSTEM_H
#define ALERTSYSTEM_H

#include <Arduino.h>


class AlertSystem {
public:
    AlertSystem();
    void checkAlerts(float temp, float objtemp, uint16_t soil, 
                     uint16_t gas, uint16_t flame, bool water);

    uint8_t getFloodAlert() const;
    uint8_t getFireAlert() const;
    uint8_t getAlertStatus() const;

private:
    void triggerAlert(String message);
    
    struct {
        float temp_high = 95.0f;    // Fahrenheit
        uint16_t soil_wet = 65;   // percentage
        uint16_t gas_high = 2000; // Analog reading  
        uint16_t flame_threshold = 2000; // Analog reading
    } thresholds;


    uint8_t floodAlert = 0;
    uint8_t fireAlert = 0;
    uint8_t alertStatus = 0;
    
    
};

#endif