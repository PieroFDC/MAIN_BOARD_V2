#include "main.hpp"

void timerCallback() {
    bool rslt = radioTX(dataToNRFStruct);
}

void setup() {
    Serial.begin(2000000);
    while (!Serial) delay(10);

    Wire.begin();
    Wire.setClock(400000);

    radioSetup();
    initESC();
    gimbalSetup();
    gpsSetup();
    // adsSetup(&Wire);
    // ultrasonicSetup();
    imuSetup(&Wire);

    timer->setOverflow(5, HERTZ_FORMAT);
    timer->attachInterrupt(timerCallback);
    timer->resume();
}

void loop() {
    // ultrasonic Handle
    // double sonic_read = ultrasonicRead();
    // dataToPcStruct.sonic = ...;
    dataToPcStruct.sonic = false;

    // Voltage Handle
    // dataToPcStruct.volt = adsRead();
    dataToPcStruct.volt = 12.5;

    imuData = imuRead();
    gpsData = gpsRead();

    dataToPcStruct.yaw = imuData[0];
    dataToPcStruct.pitch = imuData[1];
    dataToPcStruct.roll = imuData[2];
    dataToPcStruct.calibration = imuData[4];

    dataToPcStruct.lat = gpsData[0];
    dataToPcStruct.lon = gpsData[1];
    dataToPcStruct.velocity = gpsData[2];
    dataToPcStruct.course = gpsData[3];

    dataToPcStruct.yaw = yawFilter.filterData(dataToPcStruct.yaw);
    dataToPcStruct.pitch = pitchFilter.filterData(dataToPcStruct.pitch);
    dataToPcStruct.roll = rollFilter.filterData(dataToPcStruct.roll);

    dataToPcStruct.velocity = velocityFilter.filterData(dataToPcStruct.velocity);
    dataToPcStruct.course = courseFilter.filterData(dataToPcStruct.course);

    dataFromPcStruct = serialCom.getDataFromPC();

    gimbalLoop(dataFromPcStruct.gimbalyaw, dataToPcStruct.pitch, dataToPcStruct.roll);

    if(dataFromNRFStruct.startStop) {
        if(dataFromNRFStruct.mode) {
            escFeedback = writeESC(dataFromPcStruct.fl, dataFromPcStruct.fr); // Auto -> Control
        } else {
            escFeedback = writeESC(dataFromNRFStruct.pwml, dataFromNRFStruct.pwmr); // Manual -> Joystick
        }
    } else {
        escFeedback = writeESC(1500, 1500); // Stop
    }

    dataToPcStruct.pwml = escFeedback[0];
    dataToPcStruct.pwmr = escFeedback[1];

    if(dataFromNRFStruct.returnHome) {
        dataToPcStruct.nrf = "rh"; // Return Home
    } else {
        dataToPcStruct.nrf = "nrf"; // Normal
    }

    serialCom.replyToPC(dataToPcStruct); 

    // Data To NRF
    dataToNRFStruct.lat = dataToPcStruct.lat;
    dataToNRFStruct.lon = dataToPcStruct.lon;
    dataToNRFStruct.heading = dataToPcStruct.yaw;
    dataToNRFStruct.velocity = dataToPcStruct.velocity;
    dataToNRFStruct.numWaypoints = 0; //
    dataToNRFStruct.battery = dataToPcStruct.volt;
    dataToNRFStruct.sonic = dataToPcStruct.sonic;
    dataToNRFStruct.calibration = dataToPcStruct.calibration;
}