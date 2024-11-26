#include "config.h"
#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <ArduinoJson.h>
#include <secret.h>
#include <BlynkSimpleEsp32.h>
#include <datalogger.h>
#include <AlertSystem.h>

DataLogger dataLogger;
AlertSystem alertSystem;

bool notificationFlag = false;

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];
char receivedData[BUFFER_SIZE]; // Buffer to hold received data

// Variables for sensor readings
float temperature;
float humidity;
uint16_t soilMoisture;
uint16_t mq2Reading;
uint16_t flameSensor;
bool waterDetected;
double objectTemp;
double ambientTemp;
uint16_t batteryReading;

static RadioEvents_t RadioEvents;

int16_t rssi, rxSize;

bool lora_idle = true;

void setup() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);
    
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

    
    dataLogger.begin();

    rssi=0;
  
    RadioEvents.RxDone = OnRxDone;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                               LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                               LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                               0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
}



void loop(){


  if(lora_idle)
  {
    lora_idle = false;
    Serial.println("into RX mode");
    Radio.Rx(0);
  }
  Radio.IrqProcess( );

  if (notificationFlag){
    if (alertSystem.getFloodAlert() == 1){
      Blynk.logEvent("flood_risk_elevated");
    }
    if (alertSystem.getFloodAlert() == 2){
      Blynk.logEvent("flood_danger");
    }
    if (alertSystem.getFireAlert() == 1){
      Blynk.logEvent("fire_risk_elevated");
    }
    if (alertSystem.getFireAlert() == 2){
      Blynk.logEvent("fire_danger");
    }
    
    notificationFlag = false;
  }

  Blynk.run();

}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ){

    rssi=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    Radio.Sleep( );
    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n",rxpacket,rssi,rxSize);

// Parse the received JSON data
    StaticJsonDocument<BUFFER_SIZE> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, rxpacket);

    if (error) {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
        return;
    }

    // Extract values from JSON
    temperature = jsonDoc["temperature"];
    humidity = jsonDoc["humidity"];
    soilMoisture = jsonDoc["soil_moisture"];
    mq2Reading = jsonDoc["mq2"];
    waterDetected = jsonDoc["water_detected"];
    flameSensor = jsonDoc["Flame Sensor"];
    objectTemp = jsonDoc["MLX Object Temp"];
    ambientTemp = jsonDoc["MLX Ambient Temp"];
    batteryReading = jsonDoc["Battery Percentage"];

    alertSystem.checkAlerts(temperature, objectTemp, soilMoisture, mq2Reading, flameSensor, waterDetected);

    if(alertSystem.getAlertStatus() > 0){notificationFlag = true;}

    // Print the received sensor data
    Serial.println("Received Sensor Data:");
    Serial.print("Temperature: "); Serial.println(temperature);
    Serial.print("Humidity: "); Serial.println(humidity);
    Serial.print("Soil Moisture: "); Serial.println(soilMoisture);
    Serial.print("MQ2 Reading: "); Serial.println(mq2Reading);
    Serial.print("Flame Sensor: "); Serial.println(flameSensor);
    Serial.print("Water Detected: "); Serial.println(waterDetected ? "Yes" : "No");
    Serial.print("MLX Object Temp: "); Serial.println(objectTemp);
    Serial.print("MLX Ambient Temp: "); Serial.println(ambientTemp);
    Serial.print("Battery percentage "); Serial.println(batteryReading);

    Serial.println("Sending sensor data to Blynk...");
    Blynk.virtualWrite(V0, temperature);
    Blynk.virtualWrite(V1, humidity);
    Blynk.virtualWrite(V2, soilMoisture);
    Blynk.virtualWrite(V3, mq2Reading);
    Blynk.virtualWrite(V4, flameSensor);
    Blynk.virtualWrite(V5, waterDetected);
    Blynk.virtualWrite(V6, objectTemp);
    Blynk.virtualWrite(V7, ambientTemp);
    Blynk.virtualWrite(V9, batteryReading);
    Blynk.virtualWrite(V8, alertSystem.getAlertStatus());
    Blynk.virtualWrite(V10, alertSystem.getFireAlert());
    Blynk.virtualWrite(V11, alertSystem.getFloodAlert());

    Serial.println("Sending sensor data to Firebase...");
    dataLogger.logData(
        temperature,
        humidity,
        soilMoisture,
        mq2Reading,
        flameSensor,
        waterDetected,
        objectTemp,
        ambientTemp,
        rssi,
        batteryReading);
    
    
  

    Blynk.run();

    lora_idle = true;
}