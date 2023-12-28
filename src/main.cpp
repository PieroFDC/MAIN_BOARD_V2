#include "main.hpp"

void setup() {
    Serial.begin(2000000);
    while (!Serial) delay(10);

    Wire.begin();
    Wire.setClock(400000);

    attachInterrupt(IRQ_PIN, interruptNRF, FALLING);

    initESC();
    gimbalSetup();
    gpsSetup();
    // adsSetup(&Wire);
    // ultrasonicSetup();
    imuSetup(&Wire);
} 

void loop() {
    // ultrasonic Handle
    // dataToPcStruct.sonic = ultrasonicRead();
    dataToPcStruct.sonic = 10;

    // Voltage Handle
    // dataToPcStruct.volt = adsRead();
    dataToPcStruct.volt = 12;

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

    escFeedback = writeESC(dataFromPcStruct.fl, dataFromPcStruct.fr);

    dataToPcStruct.pwml = escFeedback[0];
    dataToPcStruct.pwmr = escFeedback[1];

    // NRF Handle
    dataToPcStruct.nrf = "nrf";

    serialCom.replyToPC(dataToPcStruct);
}

void interruptNRF() {
    dataFromNRFStruct = radioRX();
}