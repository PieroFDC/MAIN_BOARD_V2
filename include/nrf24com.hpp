#ifndef __NRF24COM_HPP
#define __NRF24COM_HPP

#include <nRF24L01.h>
#include <RF24.h>
#include "data.hpp"

#define CE_PIN PB12
#define CSN_PIN PA4

#define IRQ_PIN PB0

//Variable con la dirección del canal que se va a leer
uint8_t address[] = { 0xAB, 0x8F, 0xDE, 0x9C, 0x37 };

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

//vector para los datos recibidos
char RXdata[17];

void radioSetup() {
    //inicializamos el NRF24L01 
    radio.begin();
    radio.setAutoAck(1);
    radio.enableAckPayload();
    radio.maskIRQ(1, 1, 0);
    radio.setPALevel(RF24_PA_HIGH);

    //Abrimos el canal de Lectura
    radio.openReadingPipe(1, address);
    
    //empezamos a escuchar por el canal
    radio.startListening();
}

void radioTX(char TXdata[45]) {
    radio.stopListening();
    radio.write(TXdata, sizeof(TXdata));
    radio.startListening();
}

dataFromNRF radioRX() {
    dataFromNRF data;

    if (radio.available()) {    
        radio.read(RXdata, sizeof(RXdata));

        if (RXdata[0] == '<' && RXdata[sizeof(RXdata) - 1] == '>') {
            parseData(RXdata, data);
        }
    }

    return data;
}

void parseData(const char* input, dataFromNRF& result) {
    char* token = strtok(const_cast<char*>(input), ",");

    result.mode = atoi(token + 1);

    token = strtok(nullptr, ",");
    result.pwml = atoi(token);

    token = strtok(nullptr, ",");
    result.pwmr = atoi(token);

    token = strtok(nullptr, ",");
    result.startStop = atoi(token);

    token = strtok(nullptr, ">");
    result.returnHome = atoi(token);
}

#endif //__NRF24COM_HPP