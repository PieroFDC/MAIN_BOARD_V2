#ifndef __UTILS_HPP
#define __UTILS_HPP

dataFromPc dataFromPcStruct;
dataToPc dataToPcStruct;

dataFromNRF dataFromNRFStruct;
dataToNRF dataToNRFStruct;

// #include "data.hpp"

// void parseData(const char* input, dataFromNRF& result) {
//     char* token = strtok(const_cast<char*>(input), ",");

//     result.mode = atoi(token + 1);

//     token = strtok(nullptr, ",");
//     result.pwml = atoi(token);

//     token = strtok(nullptr, ",");
//     result.pwmr = atoi(token);

//     token = strtok(nullptr, ",");
//     result.startStop = atoi(token);

//     token = strtok(nullptr, ">");
//     result.returnHome = atoi(token);
// }

// void structToString(const dataToNRF& data, const char* result) {
//     // Formato: "<lat,lon,heading,velocity,numWaypoints,battery,sonic>"
//     String res;

//     Serial.print("datastruct: ");
//     Serial.println(data.lat);

//     res = "<" + String(data.lat) + String(data.lon) + ">";
//     result = res.c_str();
//     //   sprintf(result, "<%.6f,%.6f,%.2f,%.2f,%d,%.2f,%.2f>", 
//     //           data.lat, data.lon, data.heading, data.velocity, 
//     //           data.numWaypoints, data.battery, data.sonic);
// }

#endif //__UTILS_HPP