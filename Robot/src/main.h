#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <BluetoothSerial.h>

// Pin Definitions
#define LEFT_IN1 13    // Left motor PIN1
#define LEFT_IN2 12    // Left motor PIN2
#define MIDDLE_IN1 18  // Middle motor PIN1
#define MIDDLE_IN2 19  // Middle motor PIN2
#define RIGHT_IN1 25   // Right motor PIN1
#define RIGHT_IN2 26   // Right motor PIN2

// Function Prototypes
static void pinSetup();
static void setMotorSpeed(int pin1, int pin2, float speed);
static void controlMotors(float Vx, float Vy, float omega);
static void stopAllMotors();

#endif