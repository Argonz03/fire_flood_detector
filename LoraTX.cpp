#include "LoraTX.h"

char LoraTX::txpacket[256];

LoraTX* LoraTX::instance = nullptr;

RadioEvents_t LoraTX::RadioEvents;

LoraTX::LoraTX(){

  lora_idle = true;

  instance = this;

  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);

  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;

  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH, LORA_SPREADING_FACTOR, 
                    LORA_CODINGRATE, LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON, 
                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000); 


}

void LoraTX::loraTransmit(const char* message){
  lora_idle = true; //forcing lora_idle to true
  if (lora_idle) {
        delay(1000);
        sprintf(txpacket, "%s", message);
        Serial.printf("Sending packet: %s\n", txpacket);
        Radio.Send((uint8_t*)txpacket, strlen(txpacket));
        lora_idle = false;
    }

}

void LoraTX::OnTxDone() {
    Serial.println("TX done...");
    if (instance != nullptr) {
        instance->lora_idle = true; // Set the state to idle when transmission is done
    }
}

void LoraTX::OnTxTimeout() {
    Radio.Sleep();
    Serial.println("TX Timeout...");
    if (instance != nullptr) {
        instance->lora_idle = true; // Set the state to idle when transmission is done
    }

}

void LoraTX::processRadioEvents() {
    // Process any pending LoRa events
    Radio.IrqProcess();
}
