#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <Firebase_ESP_Client.h>

class DataLogger {
public:
    DataLogger();
    void begin(const char* firebaseHost, const char* firebaseAuth);
    void logData(float temp, float hum, uint16_t soil, uint16_t gas, 
                 uint16_t flame, bool water, double objTemp, 
                 double ambTemp, int16_t rssi);
private:
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
};

#endif