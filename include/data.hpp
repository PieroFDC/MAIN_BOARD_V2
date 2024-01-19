#ifndef __DATA_HPP
#define __DATA_HPP

#include <Arduino.h>

struct dataToPc {
  float lat;
  float lon;
  float velocity;
  float course;
  float yaw;
  float pitch;
  float roll;
  int calibration;
  String nrf;
  int sonic;
  float volt;
  int pwml;
  int pwmr;
};

struct dataFromPc {
  int fl;
  int fr;
  float gimbalyaw;
  String nrf;
};

struct dataFromNRF {
  bool mode;
  int pwml;
  int pwmr;
  bool startStop;
  bool returnHome;
};

// struct dataToNRF {
//   float lat;
//   float lon;
//   float heading;
//   float velocity;
//   int numWaypoints;
//   float battery;
//   float sonic;
// };

struct dataToNRF {
  double lat;
  double lon;
  double heading;
  double velocity;
  int numWaypoints;
  double battery;
  double sonic;
};

#endif //__DATA_HPP