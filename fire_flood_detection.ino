#include <DHT22.h>
#include <Wire.h>
#include <DFRobot_ADS1115.h>
#include <Adafruit_MLX90614.h>
//define pin data
#define pinDHT22 45 
#define pinSDA 34
#define pinSCL 33
#define pinWater 46


DHT22 dht22(pinDHT22); 

float valueMQ2;
float valueSoil;
float valueFlame;
int waterState;

DFRobot_ADS1115 ads(&Wire);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 
void setup() {

  Wire.begin(34, 33);
  Serial.begin(115200); //1bit=10µs

  pinMode(pinWater, INPUT);

  ads.setAddr_ADS1115(0x48);   // 0x48
  ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
  ads.setMode(eMODE_SINGLE);       // single-shot mode
  ads.setRate(eRATE_128);          // 128SPS (default)
  ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
  ads.init();

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };

  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");
}
  



void loop() {
  //Serial.println(dht22.debug()); //optionnal

  float t = dht22.getTemperature(false);
  float h = dht22.getHumidity();



  if (dht22.getLastError() != dht22.OK) {
    Serial.print("last error :");
    Serial.println(dht22.getLastError());
  }

  Serial.print("Hum: ");Serial.print(h,1);Serial.print("\t");
  Serial.print("Temp: ");Serial.println(t,1);

  if (ads.checkADS1115())
    {
        int16_t adc0, adc1, adc2, adc3;
        adc0 = ads.readVoltage(0);
        Serial.print("Soil Sensor: ");
        Serial.print(adc0);
        Serial.print("mV,  ");
        adc1 = ads.readVoltage(1);
        Serial.print("Flame Sensor: ");
        Serial.print(adc1);
        Serial.print("mV,  ");
        adc2 = ads.readVoltage(2);
        Serial.print("Gas Sensor: ");
        Serial.print(adc2);
        Serial.println("mV,  ");
        waterState = digitalRead(pinWater);
        Serial.print("Water Detected: ");
        Serial.println(waterState);

    }
    else
    {
        Serial.println("ADS1115 Disconnected!");
    }

  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF());
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");

  Serial.println();

  delay(2000); //Collecting period should be : >1.7 second
}
