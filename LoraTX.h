#ifndef LORATX
#define LORATX

#include "LoRaWan_APP.h"
#include <Arduino.h>

class LoraTX{
  public:

    LoraTX();

    void loraTransmit(const char* message);

    void processRadioEvents();


  private:

    static void OnTxDone();
    static void OnTxTimeout();

    static constexpr uint32_t RF_FREQUENCY = 915000000; // Frequency in Hz
    static constexpr int8_t TX_OUTPUT_POWER = 5; // dBm
    static constexpr uint8_t LORA_BANDWIDTH = 0; // 0: 125 kHz
    static constexpr uint8_t LORA_SPREADING_FACTOR = 7; // [SF7..SF12]
    static constexpr uint8_t LORA_CODINGRATE = 1; // [1: 4/5]
    static constexpr uint16_t LORA_PREAMBLE_LENGTH = 8;
    static constexpr bool LORA_FIX_LENGTH_PAYLOAD_ON = false;
    static constexpr bool LORA_IQ_INVERSION_ON = false;
    static constexpr uint32_t RX_TIMEOUT_VALUE = 1000;
    static constexpr size_t BUFFER_SIZE = 256;

    bool lora_idle;

    static char txpacket[256];

    static RadioEvents_t RadioEvents;

    static LoraTX* instance;


};


#endif