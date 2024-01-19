#ifndef __NRF24COM_HPP
#define __NRF24COM_HPP

#include <nRF24L01.h>
#include <RF24.h>
#include "data.hpp"

#define CE_PIN PB12
#define CSN_PIN PA4

#define IRQ_PIN PB0

const byte Address[2][5] = {{ 0xAB, 0x8F, 0xDE, 0x9C, 0x37 }, { 0xAB, 0x8F, 0xDE, 0x9C, 0x38 }};


RF24 radio(CE_PIN, CSN_PIN);

bool newRadioData = false;
char RXdata[32];
char TXdata[64];

void getRadioData();

void radioSetup() {
    radio.begin();
    radio.setDataRate( RF24_1MBPS );
    radio.setPALevel(RF24_PA_HIGH);

    radio.openWritingPipe(Address[0]);
    radio.openReadingPipe(1, Address[1]);

    radio.maskIRQ(1, 1, 0);
    attachInterrupt(IRQ_PIN , getRadioData, FALLING);
    radio.startListening();
}

bool radioTX() {
    radio.stopListening();
    delayMicroseconds(50);
    bool rslt = radio.write(&TXdata, sizeof(TXdata));
    delayMicroseconds(50);
    radio.startListening();
    delayMicroseconds(50);

    return rslt;
}

void getRadioData() {
    if (radio.available()) {    
        radio.read(&RXdata, sizeof(RXdata));
        newRadioData = true;
    }
}

#endif //__NRF24COM_HPP