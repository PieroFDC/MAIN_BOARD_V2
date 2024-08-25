#ifndef __DATA_HPP
#define __DATA_HPP

#include <Arduino.h>

struct dataToPc {
  double lat;
  double lon;
  double velocity;
  double course;
  double yaw;
  double pitch;
  double roll;
  int calibration;
  String nrf;
  bool sonic;
  double volt;
  int pwml;
  int pwmr;
};

struct dataFromPc {
  int fl;
  int fr;
  double gimbalyaw;
  int numWaypoint = 0;
  String nrf;
};

struct dataFromNRF {
  bool mode;
  int pwml;
  int pwmr;
  bool startStop;
  bool returnHome;
};

struct dataToNRF {
  double lat;
  double lon;
  double heading;
  double velocity;
  int numWaypoints;
  double battery;
  bool sonic;
  int calibration;
};

#endif //__DATA_HPP