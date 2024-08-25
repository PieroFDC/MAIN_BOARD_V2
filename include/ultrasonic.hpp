#ifndef __ULTRASONIC_HPP
#define __ULTRASONIC_HPP

#include <Arduino.h>

#define TRIG_PIN 24
#define ECHO_PIN 25

long duration;
unsigned long startTime = 0;
const int THRESHOLD_DISTANCE = 20;
const unsigned long THRESHOLD_TIME = 5000;

void ultrasonicSetup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

double ultrasonicRead() {
    digitalWrite(TRIG_PIN, LOW);
    
    delayMicroseconds(5);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    duration = pulseIn(ECHO_PIN, HIGH);
    
    return duration * 0.034 / 2;
}

bool checkContainer() {
  double sonic_read = ultrasonicRead();

  Serial.print("Sonic: ");
  Serial.println(sonic_read, 2);

  if (sonic_read < THRESHOLD_DISTANCE) {
    if (startTime == 0) {
      startTime = millis();
    } else {
      if (millis() - startTime > THRESHOLD_TIME) {
        startTime = 0;
        return true;
      }
    }
  } else {
    startTime = 0;
  }
  return false;
}

#endif //__ULTRASONIC_HPP