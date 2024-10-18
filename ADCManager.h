#ifndef ADCMANAGER_H
#define ADCMANAGER_H

#include <DFRobot_ADS1115.h>

class ADCManager {

  public:

    ADCManager(); //Class constructor

    void begin();


    void readAllSensors(); //class method


  private:
    DFRobot_ADS1115 ads; //ADS object from the DF robot library

};

#endif