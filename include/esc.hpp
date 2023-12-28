#ifndef __ESC_HPP
#define __ESC_HPP

#include <Arduino.h>
#include <Servo.h>

Servo ESCL;
Servo ESCR;

int zeroESC = 1500; // Amplitud minima de pulso para el ESC

void initESC(){
    ESCL.attach(19);
    ESCR.attach(20);

    ESCL.writeMicroseconds(zeroESC); //1000 = 1ms
    ESCR.writeMicroseconds(zeroESC); //1000 = 1ms
    delay(1000);
}

std::vector<int> writeESC(int escl, int escr) {
    
    std::vector<int> esc(2);

    escl = constrain(escl, 1100, 1900);
    escr = constrain(escr, 1100, 1900);

    ESCL.writeMicroseconds(escl);
    ESCR.writeMicroseconds(escr);

    esc[0] = escl;
    esc[1] = escr;

    return esc;
}

#endif //__ESC_HPP