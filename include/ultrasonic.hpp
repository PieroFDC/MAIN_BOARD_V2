#ifndef __ULTRASONIC_HPP
#define __ULTRASONIC_HPP

#include <Arduino.h>

#define TRIG_PIN 24
#define ECHO_PIN 25

long duration;

void ultrasonicSetup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

int ultrasonicRead() {
    digitalWrite(TRIG_PIN, LOW);
    
    delayMicroseconds(5);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    duration = pulseIn(ECHO_PIN, HIGH);
    
    return duration * 0.034 / 2;
}

#endif //__ULTRASONIC_HPP