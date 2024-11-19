#ifndef DIGITALSENSORS_H
#define DIGITALSENSORS_H

#include <Adafruit_MLX90614.h>
#include <DHT.h>

class IRTemp {

  public:
    IRTemp();
    void begin();
    void readIRTemp();
    double objectTemp();
    double ambientTemp();



  private:
    Adafruit_MLX90614 mlx;

};

class WaterDetector{

  public:
    WaterDetector(int pin);

    void takeReading();

    bool isWaterDetected();
    

  private:

    int pinWater;

};

class Temp_Hum {

  public:
    Temp_Hum(int pin);
    void takeReading();
    float temperature();
    float humidity();


  private:
    DHT dht22;

};




#endif
