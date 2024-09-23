#ifndef __IMU_HPP
#define __IMU_HPP

#include <vector>
#include <Arduino.h>
#include <Adafruit_BNO08x.h>

#define BNO08X_RESET -1

std::vector<float> mpuRead(4);

unsigned long lastIMUReadTime;
const unsigned long imuReadInterval = 500;

struct euler_t {
    float yaw;
    float pitch;
    float roll;
} ypr;

Adafruit_BNO08x  bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

sh2_SensorId_t reportType = SH2_ROTATION_VECTOR;
long reportIntervalUs = 4000;

void setReports(sh2_SensorId_t reportType, long report_interval) {
    if (!bno08x.enableReport(reportType, report_interval)) {
    }
}

void imuSetup(TwoWire *wireDevice) {
    if (!bno08x.begin_I2C(0x4B, wireDevice)) {
        Serial.println("Failed to find IMU chip");
        delay(500);
        NVIC_SystemReset();
    }

    Serial.println("IMU Found!");

    setReports(reportType, reportIntervalUs);
    delayMicroseconds(100);
    lastIMUReadTime = millis();
}

void quaternionToEuler(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false) {

    float sqr = sq(rotational_vector->real);
    float sqi = sq(rotational_vector->i);
    float sqj = sq(rotational_vector->j);
    float sqk = sq(rotational_vector->k);

    ypr->yaw = atan2(2.0 * (rotational_vector->i * rotational_vector->j + rotational_vector->k * rotational_vector->real), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (rotational_vector->i * rotational_vector->k - rotational_vector->j * rotational_vector->real) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (rotational_vector->j * rotational_vector->k + rotational_vector->i * rotational_vector->real), (-sqi - sqj + sqk + sqr));

    if(degrees) {
      ypr->yaw *= RAD_TO_DEG;
      ypr->pitch *= RAD_TO_DEG;
      ypr->roll *= RAD_TO_DEG;
    }
}

std::vector<float> imuRead() {

    unsigned long currentTime = millis();

    if (bno08x.wasReset()) {
        Serial.print("sensor was reset ");
        setReports(reportType, reportIntervalUs);
    }

    if (currentTime - lastIMUReadTime > imuReadInterval) {
        delay(500);
        Serial.println(currentTime - lastIMUReadTime);
        NVIC_SystemReset();
    }

    lastIMUReadTime = currentTime;

    if (bno08x.getSensorEvent(&sensorValue)) {
        quaternionToEuler(&sensorValue.un.rotationVector, &ypr, true);
        mpuRead[0] = ypr.yaw;
        mpuRead[1] = ypr.pitch;
        mpuRead[2] = ypr.roll;
        mpuRead[3] = sensorValue.status;
    }

    return mpuRead;
}

#endif //__IMU_HPP