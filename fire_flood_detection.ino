#include <Wire.h>
#include "DigitalSensors.h"
#include "ADCManager.h" 
#include "LoRaWan_APP.h"
#include <ArduinoJson.h>

//define pin data
#define pinDHT22 45 
#define pinSDA 34
#define pinSCL 33
#define pinWater 46

#define RF_FREQUENCY 915000000 // Hz

#define TX_OUTPUT_POWER 5        // dBm

#define LORA_BANDWIDTH 0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR 7         // [SF7..SF12]
#define LORA_CODINGRATE 1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH 8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false


#define RX_TIMEOUT_VALUE 1000
#define BUFFER_SIZE 256 // Define the payload size here

char jsonBuffer[BUFFER_SIZE];

char txpacket[BUFFER_SIZE];

bool lora_idle=true;

static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );

ADCManager ADC;

IRTemp IRT;

WaterDetector WD(pinWater);

Temp_Hum TH(pinDHT22);

StaticJsonDocument<BUFFER_SIZE> jsonDoc;



void setup() {

  Serial.begin(115200); //1bit=10µs

  Wire.begin(34, 33);

  Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);

  ADC.begin();

  IRT.begin();

  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
    
  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                     LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                     LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                     true, 0, 0, LORA_IQ_INVERSION_ON, 3000 ); 

  
}

void loop() {

  // Hardcoded test data
  float temperature = 24.5;
  float humidity = 60.0;
  uint16_t soilMoisture = 350;
  uint16_t mq2Reading = 200;
  bool waterDetected = true;
  float flameSensor = 100.0;
  float objectTemp = 25.3;
  float ambientTemp = 22.8;

  ADC.readAllSensors();

  IRT.readIRTemp();

  WD.takeReading();

  TH.takeReading();

  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  jsonDoc["soil_moisture"] = soilMoisture;
  jsonDoc["mq2"] = mq2Reading;
  jsonDoc["water_detected"] = waterDetected;
  jsonDoc["Flame Sensor"] = flameSensor;
  jsonDoc["MLX Object Temp"] = objectTemp;
  jsonDoc["MLX Ambient Temp"] = ambientTemp;

  serializeJson(jsonDoc, jsonBuffer);

  if(lora_idle == true){
    Radio.Send((uint8_t *)jsonBuffer, strlen(jsonBuffer));
    lora_idle = false;

  }




  delay(2000); 

  Radio.IrqProcess( );



  
}


void OnTxDone( void )
{
	Serial.println("TX done......");
	lora_idle = true;
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    Serial.println("TX Timeout......");
    lora_idle = true;
}
