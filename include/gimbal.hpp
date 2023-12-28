#ifndef __GIMBAL_HPP
#define __GIMBAL_HPP

#include <Arduino.h>
#include <Servo.h>

Servo servo_yaw;
Servo servo_roll;
Servo servo_pitch;

int rollMin = 78;
int rollMax = 128;
int yawMin = 53;
int yawMax = 103;
int pitchMin = 50;
int pitchMax = 95;

void gimbalSetup() {
    servo_yaw.attach(28);   // PB13
    servo_pitch.attach(29); // PB14
    servo_roll.attach(30);  // PB15
    
    servo_yaw.write(78);
    servo_pitch.write(65);
    servo_roll.write(103);
}

void moveServo(Servo &servo, int angle, int minAngle, int maxAngle) {
  // Limitar el ángulo dentro del rango permitido
  angle = constrain(angle, minAngle, maxAngle);

  if (angle >= minAngle && angle <= maxAngle) {
    servo.write(angle);
  }
}

void gimbalLoop(float yaw, float pitch, float roll) {
    int servoPitchValue = map(pitch, -90, 90, 0, 180) - 25;
    int servoRollValue = map(roll, -90, 90, 0, 180) + 13;

    moveServo(servo_yaw, yaw, yawMin, yawMax);
    moveServo(servo_pitch, servoPitchValue, pitchMin, pitchMax);
    moveServo(servo_roll, servoRollValue, rollMin, rollMax);
}

#endif //__GIMBAL_HPP