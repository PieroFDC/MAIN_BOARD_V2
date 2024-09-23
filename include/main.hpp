#ifndef __MAIN_HPP
#define __MAIN_HPP

#include <Arduino.h>
#include <Servo.h>
#include <vector>
#include <math.h>
#include <HardwareTimer.h>
#include "esc.hpp"
#include "gps.hpp"
#include "imu.hpp"
#include "filter.hpp"
#include "gimbal.hpp"
#include "data.hpp"
#include "serialcom.hpp"
#include "nrf24com.hpp"
#include "utils.hpp"
#include "ultrasonic.hpp"
#include "battery.hpp"

bool newDataToNRF = false;

HardwareTimer *timer = new HardwareTimer(TIM4);

SerialCommunication serialCom(Serial);

Filter yawFilter(20, 0.5);
Filter pitchFilter(20, 0.5);
Filter rollFilter(20, 0.5);
Filter courseFilter(5, 0.5);
Filter velocityFilter(5, 0.5);

std::vector<float> imuData(4);
std::vector<double> gpsData(4);

std::vector<int> escFeedback(2);

void sendDataToNRF() {
  if(newDataToNRF) {
    newDataToNRF = false;
    bool rslt = radioTX(dataToNRFStruct);

    if(!rslt) {
        Serial.println("Error al enviar (NRF)");
    }
  }
}

#endif //__MAIN_HPP