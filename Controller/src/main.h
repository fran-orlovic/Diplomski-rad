#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Function Prototypes
static void readIMUData(float &Vx, float &Vy, float &omega);

#endif