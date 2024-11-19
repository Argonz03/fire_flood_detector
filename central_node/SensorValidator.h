#include <Arduino.h>

class SensorValidator {
public:
    SensorValidator();
    bool validateReadings(float temp, float hum, uint16_t soil, 
                         uint16_t gas, uint16_t flame);
private:
    struct {
        float temp_min = -40.0f, temp_max = 125.0f;
        float hum_min = 0.0f, hum_max = 100.0f;
        uint16_t soil_min = 0, soil_max = 3300;
        uint16_t gas_min = 0, gas_max = 4095;
        uint16_t flame_min = 0, flame_max = 4095;
    } ranges;

    uint8_t errorCounts[6];
    const uint8_t ERROR_THRESHOLD = 3;
};

#endif