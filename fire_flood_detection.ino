#include <Wire.h>
#include "DigitalSensors.h"
#include "ADCManager.h" 
#include "LoraTX.h"
#include <ArduinoJson.h>

//define pin data
#define pinDHT22 45 
#define pinSDA 34
#define pinSCL 33
#define pinWater 46

#define BUFFER_SIZE 256

char jsonBuffer[BUFFER_SIZE];

ADCManager ADC;

IRTemp IRT;

WaterDetector WD(pinWater);

Temp_Hum TH(pinDHT22);

LoraTX lora;

StaticJsonDocument<BUFFER_SIZE> jsonDoc;



void setup() {

  Serial.begin(115200); //1bit=10µs

  Wire.begin(34, 33);

  ADC.begin();

  IRT.begin();


  
}

void loop() {

  // Hardcoded test data
  float temperature = 24.5;
  float humidity = 60.0;
  uint16_t soilMoisture = 350;
  uint16_t mq2Reading = 200;
  bool waterDetected = true;
  float flameSensor = 100.0;
  float objectTemp = 25.3;
  float ambientTemp = 22.8;

  ADC.readAllSensors();

  IRT.readIRTemp();

  WD.takeReading();

  TH.takeReading();

  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  jsonDoc["soil_moisture"] = soilMoisture;
  jsonDoc["mq2"] = mq2Reading;
  jsonDoc["water_detected"] = waterDetected;
  jsonDoc["Flame Sensor"] = flameSensor;
  jsonDoc["MLX Object Temp"] = objectTemp;
  jsonDoc["MLX Ambient Temp"] = ambientTemp;

  serializeJson(jsonDoc, jsonBuffer);

  lora.loraTransmit(jsonBuffer);

  


  delay(2000); //Collecting period should be : >1.7 second

  lora.processRadioEvents();

  
}
