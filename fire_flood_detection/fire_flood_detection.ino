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

  Serial.begin(115200);

  pinMode(transistorPin, OUTPUT);
  digitalWrite(transistorPin, HIGH); //turn on sensors

  delay(1000 * 60 * 2); //2 minute delay to allow for gas sensor to warm up

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

  esp_sleep_enable_timer_wakeup(13 * 60 * 1000000);//Minutes * seconds * microseconds

  

 
}

void loop() {



  uint16_t soilMoisture = getSoilMoisturePercentage(ADC.soilReading());
  uint16_t mq2Reading = ADC.gasReading();
  uint16_t flameSensor = ADC.flameReading();
  bool waterDetected = WD.isWaterDetected();
  double objectTemp = IRT.objectTemp();
  double ambientTemp = IRT.ambientTemp();
  float temperature = TH.temperature();
  float humidity = TH.humidity();
  int batteryReading = batteryPercentage();

  //ADC.readAllSensors();
  //IRT.readIRTemp();
  //WD.takeReading();
  //TH.takeReading();

  

  if(lora_idle == true){

    jsonDoc["temperature"] = temperature;
    jsonDoc["humidity"] = humidity;
    jsonDoc["soil_moisture"] = soilMoisture;
    jsonDoc["mq2"] = mq2Reading;
    jsonDoc["water_detected"] = waterDetected;
    jsonDoc["Flame Sensor"] = flameSensor;
    jsonDoc["MLX Object Temp"] = objectTemp;
    jsonDoc["MLX Ambient Temp"] = ambientTemp;
    jsonDoc["Battery Percentage"] = batteryReading;

    serializeJson(jsonDoc, jsonBuffer);

    Serial.println(jsonBuffer);
    Radio.Send((uint8_t *)jsonBuffer, strlen(jsonBuffer));
    lora_idle = false;

  }

  Radio.IrqProcess( );

}


void OnTxDone( void )
{
	Serial.println("TX done......");
	lora_idle = true;
  esp_deep_sleep_start();
  digitalWrite(transistorPin, LOW);
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    Serial.println("TX Timeout......");
    lora_idle = true;
    esp_deep_sleep_start();
}

int batteryPercentage() {

    const float referenceVoltage = 3.3; // ADC reference voltage 
    const int adcResolution = 1040;     // ADC resolution 
    const float batteryMaxVoltage = 3.39; // Fully charged voltage of the battery
    const float batteryMinVoltage = 0; // Fully discharged voltage of the battery
    
    // Read raw analog value from the battery pin
    int batteryRaw = analogRead(pinBattery);

    // Convert raw value to voltage
    float batteryVoltage = (batteryRaw * referenceVoltage) / adcResolution;

    // Map voltage to a percentage
    int percentage = map(batteryVoltage * 100, batteryMinVoltage * 100, batteryMaxVoltage * 100, 0, 100);

    
    percentage = constrain(percentage, 0, 100);


    return percentage;
}

float getSoilMoisturePercentage(uint16_t soil) {
    const uint16_t maxSoilValue = 2025; // Dry soil (0% moisture)
    const uint16_t minSoilValue = 850;  // Wet soil (100% moisture)

    // Prevent division by zero
    if (maxSoilValue == minSoilValue) return 0;

    // Calculate percentage
    float percentageSoil = ((float)(maxSoilValue - soil) / (maxSoilValue - minSoilValue)) * 100;


    percentageSoil = constrain(percentageSoil, 0, 100);

    return percentageSoil;
}

