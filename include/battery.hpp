#ifndef __BATTERY_HPP
#define __BATTERY_HPP

#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

void adsSetup(TwoWire *wireDevice) {
    if (!ads.begin(ADS1X15_ADDRESS, wireDevice)) {
        Serial.println("Failed to initialize ADS.");
        delay(500);
        NVIC_SystemReset();
    }
}

float adsRead() {
    int16_t results;
    float volt;
    float multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */
    
    results = ads.readADC_Differential_0_1();
    volt = results * multiplier * 1000;

    return volt;
}

#endif //__BATTERY_HPP