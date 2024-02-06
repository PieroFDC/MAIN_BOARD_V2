#ifndef __NRF24COM_HPP
#define __NRF24COM_HPP

#include <nRF24L01.h>
#include <RF24.h>
#include "data.hpp"
#include "utils.hpp"

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

    // radio.setAutoAck(false);
    // radio.disableAckPayload();

    radio.setDataRate( RF24_1MBPS );
    radio.setPALevel(RF24_PA_HIGH);

    radio.openWritingPipe(Address[0]);
    radio.openReadingPipe(1, Address[1]);

    // radio.setRetries(0, 0);

    radio.maskIRQ(1, 1, 0);
    attachInterrupt(IRQ_PIN , getRadioData, FALLING);
    radio.startListening();
}

bool radioTX(dataToNRF dataToNRFStruct) {
    String joinDataString = "<" + String(dataToNRFStruct.lat) + "," + String(dataToNRFStruct.lon) +
                            "," + String(dataToNRFStruct.heading) + "," + String(dataToNRFStruct.velocity) +
                            "," + String(dataToNRFStruct.numWaypoints) + "," + String(dataToNRFStruct.battery) +
                            "," + String(dataToNRFStruct.sonic) + ">";

    joinDataString.toCharArray(TXdata, sizeof(TXdata));

    radio.stopListening();
    delayMicroseconds(50);
    bool rslt = radio.write(&TXdata, sizeof(TXdata));
    delayMicroseconds(50);
    radio.startListening();
    delayMicroseconds(50);

    return rslt;
}



void parseData(const char* input, dataFromNRF& result) {
    char* token = strtok(const_cast<char*>(input), ",");

    result.mode = atoi(token);

    token = strtok(nullptr, ",");
    result.pwml = atoi(token);

    token = strtok(nullptr, ",");
    result.pwmr = atoi(token);

    token = strtok(nullptr, ",");
    result.startStop = atoi(token);

    result.returnHome = atoi(token);
}

String processString(const String &inputString) {
  bool foundLessThan = false;
  String contentBetweenLessGreater;

  for (unsigned int i = 0; i < inputString.length(); ++i) {
    char currentCharacter = inputString[i];

    if (currentCharacter == '<') {
      if (foundLessThan) {
        foundLessThan = false;
        contentBetweenLessGreater = "";
      }

      foundLessThan = true;

    } else if (currentCharacter == '>') {
      if (foundLessThan) {
        return contentBetweenLessGreater;
      }

    } else if (foundLessThan) {
      contentBetweenLessGreater += currentCharacter;
    }
  }
  return "";
}


void getRadioData() {
    String readData;

    if (radio.available()) {    
        radio.read(&RXdata, sizeof(RXdata));
        readData = processString(String(RXdata));

        if(readData.length() > 0) {
            parseData(readData.c_str(), dataFromNRFStruct);
        }
    }
}

#endif //__NRF24COM_HPP