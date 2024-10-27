#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <ArduinoJson.h>
#include "config.h"




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

static RadioEvents_t RadioEvents;

int16_t rssi, rxSize;

bool lora_idle = true;

void setup() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);
    

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

    lora_idle = true;
}