#ifndef ADCMANAGER_H
#define ADCMANAGER_H

#include <DFRobot_ADS1115.h>

class ADCManager {

  public:

    ADCManager(); //Class constructor

    void begin();


    void readAllSensors(); //class method

    uint16_t flameReading();
    uint16_t gasReading();
    uint16_t soilReading();


  private:
    DFRobot_ADS1115 ads; //ADS object from the DF robot library

};

#endif