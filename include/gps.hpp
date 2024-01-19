#ifndef __GPS_HPP
#define __GPS_HPP

#include <vector>
#include <Arduino.h>
#include <TinyGPSPlus.h>

std::vector<double> gpsValues(4);
std::vector<double> nullVector(4, -999.0);
TinyGPSPlus gps;
HardwareSerial Serial2(USART2);

void gpsSetup() {
    Serial2.begin(230400);
}

std::vector<double> gpsRead() {
    while (Serial2.available() > 0) {
        if (gps.encode(Serial2.read())) {
            if (gps.location.isValid()) {
                gpsValues[0] = gps.location.lat();
                gpsValues[1] = gps.location.lng();
                gpsValues[2] = gps.speed.mps();
                gpsValues[3] = gps.course.deg();
            }
            else {
                gpsValues = nullVector;
            }
        }
    }

    if (millis() > 5000 && gps.charsProcessed() < 10) {
        Serial.println(F("GPS ERROR"));
    }
    return gpsValues;
}

#endif //__GPS_HPP