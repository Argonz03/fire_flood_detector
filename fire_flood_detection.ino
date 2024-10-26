#include <Wire.h>
#include "DigitalSensors.h"
#include "ADCManager.h" 
#include "LoRaWan_APP.h"
#include <ArduinoJson.h>
#include "config.h"


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

  uint16_t soilMoisture = ADC.soilReading();
  uint16_t mq2Reading = ADC.gasReading();
  uint16_t flameSensor = ADC.flameReading();
  bool waterDetected = WD.isWaterDetected();
  double objectTemp = IRT.objectTemp();
  double ambientTemp = IRT.ambientTemp();
  float temperature = TH.temperature();
  float humidity = TH.humidity();

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


  Radio.IrqProcess( );


  delay(30000); 

  

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
