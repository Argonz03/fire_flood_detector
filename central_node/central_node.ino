#include <LoRaWan_APP.h>
#include "Arduino.h"
#include <ArduinoJson.h>
#include "config.h"
#include "SensorValidator.h"
#include "AlertSystem.h"
#include "DataLogger.h"

// Create instances
SensorValidator validator;
AlertSystem alertSystem;
DataLogger dataLogger;

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];
char receivedData[BUFFER_SIZE]; // Buffer to hold received data

// Variables for sensor readings
float temperature;
float humidity;
uint16_t soilMoisture;
uint16_t mq2Reading;
uint16_t flameSensor;
bool waterDetected;
double objectTemp;
double ambientTemp;

static RadioEvents_t RadioEvents;

int16_t rssi, rxSize;

bool lora_idle = true;

void setup() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TYPE);

    rssi = 0;

    // Initialize Radio
    RadioEvents.RxDone = OnRxDone;
    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                       LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                       LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                       0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

    // Initialize DataLogger with Firebase configurations
    dataLogger.begin(FIREBASE_HOST, FIREBASE_AUTH);

    // Initialize Alert System
    alertSystem.begin();
}

void loop() {
    if (lora_idle) {
        lora_idle = false;
        Serial.println("into RX mode");
        Radio.Rx(0);
    }
    Radio.IrqProcess();
}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
    rssi = rssi; // Store RSSI value
    rxSize = size; // Store received size
    memcpy(rxpacket, payload, size); // Copy payload to rxpacket
    rxpacket[size] = '\0'; // Null-terminate the string
    Radio.Sleep(); // Put radio to sleep

    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n", rxpacket, rssi, rxSize);

    // Parse the received JSON data
    StaticJsonDocument<BUFFER_SIZE> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, rxpacket);

    if (error) {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
        return; // Exit if parsing fails
    }

    // Extract values from JSON
    temperature = jsonDoc["temperature"];
    humidity = jsonDoc["humidity"];
    soilMoisture = jsonDoc["soil_moisture"];
    mq2Reading = jsonDoc["mq2"];
    waterDetected = jsonDoc["water_detected"];
    flameSensor = jsonDoc["Flame Sensor"];
    objectTemp = jsonDoc["MLX Object Temp"];
    ambientTemp = jsonDoc["MLX Ambient Temp"];

    // Print the received sensor data
    Serial.println("Received Sensor Data:");
    Serial.print("Temperature: "); Serial.println(temperature);
    Serial.print("Humidity: "); Serial.println(humidity);
    Serial.print("Soil Moisture: "); Serial.println(soilMoisture);
    Serial.print("MQ2 Reading: "); Serial.println(mq2Reading);
    Serial.print("Flame Sensor: "); Serial.println(flameSensor);
    Serial.print("Water Detected: "); Serial.println(waterDetected ? "Yes" : "No");
    Serial.print("MLX Object Temp: "); Serial.println(objectTemp);
    Serial.print("MLX Ambient Temp: "); Serial.println(ambientTemp);

    // Validate readings before logging or processing them further
    if (validator.validateReadings(temperature, humidity, soilMoisture, mq2Reading, flameSensor)) {
        // Log valid data to Firebase
        dataLogger.logData(temperature, humidity, soilMoisture,
                           mq2Reading, flameSensor, waterDetected,
                           objectTemp, ambientTemp, rssi);
        
        // Check for alerts based on the readings
        alertSystem.checkAlerts(temperature, humidity, soilMoisture,
                                mq2Reading, flameSensor, waterDetected);
        
        lora_idle = true; // Set LoRa to idle state after processing
    } else {
        Serial.println("One or more sensor readings are invalid.");
        lora_idle = true; // Set LoRa to idle state even on invalid readings
    }
}