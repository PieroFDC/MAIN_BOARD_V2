#include "main.hpp"

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
} 

void loop() {
    if(newRadioData) {
        parseData(RXdata, dataFromNRFStruct);
        newRadioData = false;
        Serial.print("Mode: ");
        Serial.println(dataFromNRFStruct.mode);
        Serial.print("PWML: ");
        Serial.println(dataFromNRFStruct.pwml);
    }

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


    // Data To NRF
    dataToNRFStruct.lat = dataToPcStruct.lat;
    dataToNRFStruct.lon = dataToPcStruct.lon;
    dataToNRFStruct.heading = dataToPcStruct.yaw;
    dataToNRFStruct.velocity = dataToPcStruct.velocity;
    dataToNRFStruct.numWaypoints = 0; //
    dataToNRFStruct.battery = dataToPcStruct.volt;
    dataToNRFStruct.sonic = dataToPcStruct.sonic;

    Serial.print("datastruct: ");
    Serial.println(dataToNRFStruct.lat, 6);

    // Send To NRF
    // structToString(dataToNRFStruct, TXdata);
    String res = "<" + String(dataToNRFStruct.lat) + String(dataToNRFStruct.lon) + ">";
    Serial.println(res);

    sprintf(TXdata, "<%d,%lf,%lf,%lf,%d,%lf,%lf>", 
            dataToNRFStruct.lat, dataToNRFStruct.lon, dataToNRFStruct.heading, dataToNRFStruct.velocity, 
            dataToNRFStruct.numWaypoints, dataToNRFStruct.battery, dataToNRFStruct.sonic);


    bool rslt = radioTX();
    if(rslt) {
        Serial.print("nrf: ");
        Serial.println(TXdata);
    } else {
        Serial.println("Error al enviar los datos");
    }
}