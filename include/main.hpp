#ifndef __MAIN_HPP
#define __MAIN_HPP

#include <Arduino.h>
#include <Servo.h>
#include <vector>
#include <math.h>
#include "esc.hpp"
#include "gps.hpp"
#include "imu.hpp"
#include "filter.hpp"
#include "gimbal.hpp"
#include "data.hpp"
#include "serialcom.hpp"
#include "nrf24com.hpp"
// #include "battery.hpp"
// #include "ultrasonic.hpp"

SerialCommunication serialCom(Serial);

Filter yawFilter(20, 0.5);
Filter pitchFilter(20, 0.5);
Filter rollFilter(20, 0.5);
Filter courseFilter(20, 0.5);
Filter velocityFilter(20, 0.5);

dataFromPc dataFromPcStruct;
dataToPc dataToPcStruct;

dataFromNRF dataFromNRFStruct;
dataToNRF dataToNRFStruct;

std::vector<float> imuData(4);
std::vector<double> gpsData(4);

std::vector<int> escFeedback(2);

#endif //__MAIN_HPP