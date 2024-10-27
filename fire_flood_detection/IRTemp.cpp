#include "DigitalSensors.h"
#include <Arduino.h>


IRTemp::IRTemp() : mlx() {

}

void IRTemp::begin(){

  if (!mlx.begin()) {//debugging function
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  }

}

void IRTemp::readIRTemp(){

  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF());
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");

}

double IRTemp::objectTemp(){
  double reading = mlx.readObjectTempF();
  return reading;

}

double IRTemp::ambientTemp(){
  double reading = mlx.readAmbientTempF();
  return reading;

}
