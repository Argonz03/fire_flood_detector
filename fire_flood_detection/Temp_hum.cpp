#include "DigitalSensors.h"
#include <Arduino.h>

Temp_Hum::Temp_Hum(int pin) : dht22(pin ){}

  


void Temp_Hum::takeReading(){//debugging function

  float t = dht22.getTemperature(false);
  float h = dht22.getHumidity();



  if (dht22.getLastError() != dht22.OK) {
    Serial.print("last error :");
    Serial.println(dht22.getLastError());
  }

  Serial.print("Hum: ");Serial.print(h,1);Serial.print("\t");
  Serial.print("Temp: ");Serial.println(t,1);



  
}

float Temp_Hum::temperature(){
  return dht22.getTemperature(false);

}


float Temp_Hum::humidity(){
  return dht22.getHumidity();

}