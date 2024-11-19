#include <Arduino.h>

class AlertSystem {
public:
    AlertSystem();
    void begin();
    void checkAlerts(float temp, float hum, uint16_t soil, 
                     uint16_t gas, uint16_t flame, bool water);
private:
    void triggerAlert(String message);
    
    struct {
        float temp_high = 85.0f;    // Fahrenheit
        float hum_high = 80.0f;     // Percent
        uint16_t soil_wet = 2500;   // Analog reading
        uint16_t gas_high = 2000;   // PPM
        uint16_t flame_threshold = 1000; // Analog reading
    } thresholds;
    
    const int ALERT_LED = 13;
    const int ALERT_BUZZER = 12;
};

#endif