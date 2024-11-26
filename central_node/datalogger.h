
#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <Firebase_ESP_Client.h>
#include <time.h>


class DataLogger {
public:
    DataLogger();
    void begin();
    void logData(float temp, float hum, uint16_t soil, uint16_t gas, 
                 uint16_t flame, bool water, double objTemp, 
                 double ambTemp, int16_t rssi, uint16_t batteryReading);

    struct DateTime {
        String fullDate; // Date as "YYYY-MM-DD"
        String fullTime; // Time as "HH:MM:SS"
       
    };

private:
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;


  DateTime getCurrentDateTime();


};

#endif
