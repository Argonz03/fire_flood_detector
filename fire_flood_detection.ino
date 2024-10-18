#include <Wire.h>
#include "DigitalSensors.h"
#include "ADCManager.h" 


//define pin data
#define pinDHT22 45 
#define pinSDA 34
#define pinSCL 33
#define pinWater 46



ADCManager ADC;

IRTemp IRT;

WaterDetector WD(pinWater);

Temp_Hum TH(pinDHT22);


void setup() {

  Serial.begin(115200); //1bit=10µs

  Wire.begin(34, 33);

  ADC.begin();

  IRT.begin();


  
}

void loop() {

  ADC.readAllSensors();

  IRT.readIRTemp();

  WD.takeReading();

  TH.takeReading();


  delay(2000); //Collecting period should be : >1.7 second
}
