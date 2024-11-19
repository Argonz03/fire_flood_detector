#include "OledDisplay.h"

// Constructor initializes the display with specific dimensions and reset pin
OledDisplay::OledDisplay() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void OledDisplay::begin() {
    // Initialize with I2C address
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    display.clearDisplay(); // Clear the display buffer
}

void OledDisplay::displayData(float temperature, float humidity, uint16_t soilMoisture, uint16_t mq2Reading, bool waterDetected, uint16_t flameSensor, double objectTemp, double ambientTemp) {
    display.clearDisplay(); // Clear previous data
    display.setTextSize(1); // Set text size
    display.setTextColor(SSD1306_WHITE); // Set text color
    display.setCursor(0, 0); // Set cursor position

    // Display sensor data
    display.print("Temp: "); 
    display.println(temperature);
    display.print("Humidity: ");
    display.println(humidity);
    display.print("Soil Moisture: ");
    display.println(soilMoisture);
    display.print("MQ2 Reading: ");
    display.println(mq2Reading);
    display.print("Water Detected: ");
    display.println(waterDetected ? "Yes" : "No");
    display.print("Flame Sensor: ");
    display.println(flameSensor);
    display.print("Obj Temp: ");
    display.println(objectTemp);
    display.print("Ambient Temp: ");
    display.println(ambientTemp);

    display.display(); // Update the OLED with new data
}