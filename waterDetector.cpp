#include "DigitalSensors.h"
#include <Arduino.h>

WaterDetector::WaterDetector(int pin){

  pinWater = pin;

  pinMode(pinWater, INPUT);

}

void WaterDetector::takeReading(){
  int waterState;

  waterState = digitalRead(pinWater);
  Serial.print("Water Detected: ");
  Serial.println(waterState);

}

bool WaterDetector::isWaterDetected(){
  return digitalRead(pinWater);
}