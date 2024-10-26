#include <Arduino.h>
#include "ADCManager.h"

ADCManager::ADCManager() : ads(&Wire) {}  

void ADCManager::begin(){

  ads.setAddr_ADS1115(0x48);      // Set ADS1115 I2C address
  ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
  ads.setMode(eMODE_SINGLE);      // Single-shot mode
  ads.setRate(eRATE_128);         // 128 samples per second (default)
  ads.setOSMode(eOSMODE_SINGLE);  // Set to start a single conversion
  ads.init();                     // Initialize the ADS1115

}

void ADCManager::readAllSensors(){//debugging function

    if (ads.checkADS1115())
    {        
        uint16_t adc0, adc1, adc2, adc3;
        adc0 = ads.readVoltage(0);
        Serial.print("Soil Sensor: ");
        Serial.print(adc0);
        Serial.print("mV,  ");
        adc1 = ads.readVoltage(1);
        Serial.print("Flame Sensor: ");
        Serial.print(adc1);
        Serial.print("mV,  ");
        adc2 = ads.readVoltage(2);
        Serial.print("Gas Sensor: ");
        Serial.print(adc2);
        Serial.println("mV,  ");
       

    }
    else
    {
        Serial.println("ADS1115 Disconnected!");
    }
    
}


uint16_t ADCManager::flameReading(){
  uint16_t reading = ads.readVoltage(1);
  return reading;

}

uint16_t ADCManager::gasReading(){
  uint16_t reading = ads.readVoltage(2);
  return reading;

  
}

uint16_t ADCManager::soilReading(){
  uint16_t reading = ads.readVoltage(0);
  return reading;

  
}