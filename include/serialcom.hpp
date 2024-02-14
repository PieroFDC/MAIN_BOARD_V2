#ifndef __SERIALCOM_HPP
#define __SERIALCOM_HPP

#include <Arduino.h>
#include "data.hpp"

class SerialCommunication {
private:
    const char startMarker = '<';
    const char endMarker = '>';
    const int buffSize = 64;
    char inputBuffer[64];
    int bytesRecvd = 0;
    bool readInProgress = false;
    bool newDataFromPC = false;

    int FL;
    int FR;
    float gimbalYaw;
    String nrfMessage;

    HardwareSerial &SerialDevice;

    dataFromPc parseData() {
        dataFromPc dataParse;
        char *strtokIndx;
        strtokIndx = strtok(inputBuffer, ",");
        dataParse.fl = atoi(strtokIndx);

        strtokIndx = strtok(NULL, ",");
        dataParse.fr = atoi(strtokIndx);

        strtokIndx = strtok(NULL, ",");
        dataParse.gimbalyaw = atof(strtokIndx);

        strtokIndx = strtok(NULL, ",");
        dataParse.nrf = String(strtokIndx);

        return dataParse;
    }

public:
    SerialCommunication(HardwareSerial &serialDevice) : SerialDevice(serialDevice) {}

    dataFromPc getDataFromPC() {
        dataFromPc dataPc;
        if (SerialDevice.available() > 0) {
        char x = SerialDevice.read();

        if (x == endMarker) {
            readInProgress = false;
            newDataFromPC = true;
            inputBuffer[bytesRecvd] = 0;
            dataPc = parseData();
        }

        if (readInProgress) {
            inputBuffer[bytesRecvd] = x;
            bytesRecvd++;
            if (bytesRecvd == buffSize) {
            bytesRecvd = buffSize - 1;
            }
        }

        if (x == startMarker) {
            bytesRecvd = 0;
            readInProgress = true;
        }
        }

        return dataPc;
    }

    void replyToPC(dataToPc data) {
        if (newDataFromPC) {
            newDataFromPC = false;
            SerialDevice.print("<");
            SerialDevice.print("Lat: ");
            SerialDevice.print(data.lat, 7);
            SerialDevice.print(", Lon: ");
            SerialDevice.print(data.lon, 7);
            SerialDevice.print(", Speed: ");
            SerialDevice.print(data.velocity, 4);
            SerialDevice.print(", Course: ");
            SerialDevice.print(data.course, 2);
            SerialDevice.print(", Yaw: ");
            SerialDevice.print(data.yaw, 2);
            SerialDevice.print(", Pitch: ");
            SerialDevice.print(data.pitch, 2);
            SerialDevice.print(", Roll: ");
            SerialDevice.print(data.roll, 2);
            SerialDevice.print(", NRF: ");
            SerialDevice.print(data.nrf);
            SerialDevice.print(", sonic: ");
            SerialDevice.print(data.sonic);
            SerialDevice.print(", volt: ");
            SerialDevice.print(data.volt, 2);
            SerialDevice.print(", PWML: ");
            SerialDevice.print(data.pwml);
            SerialDevice.print(", PWMR: ");
            SerialDevice.print(data.pwmr);
            SerialDevice.print(">");
        }
    }
};

#endif //__SERIALCOM_HPP