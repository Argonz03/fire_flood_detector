#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // I2C address for the OLED display

class OledDisplay {
public:
    OledDisplay();
    void begin();
    void displayData(float temperature, float humidity, uint16_t soilMoisture, uint16_t mq2Reading, bool waterDetected, uint16_t flameSensor, double objectTemp, double ambientTemp);
    
private:
    Adafruit_SSD1306 display;
};

#endif // OLED_DISPLAY_H