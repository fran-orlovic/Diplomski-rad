#include <Arduino.h>
#include <BluetoothSerial.h>

#define LEFT_IN1 13 // Left motor PIN1
#define LEFT_IN2 12 // Left motor PIN2
#define MIDDLE_IN1 18 // Middle motor PIN1
#define MIDDLE_IN2 19 // Middle motor PIN2
#define RIGHT_IN1 25 // Right motor PIN1
#define RIGHT_IN2 26 // Right motor PIN2

static void pinSetup()
{
    pinMode(LEFT_IN1, OUTPUT);
    pinMode(LEFT_IN2, OUTPUT);
    pinMode(MIDDLE_IN1, OUTPUT);
    pinMode(MIDDLE_IN2, OUTPUT);
    pinMode(RIGHT_IN1, OUTPUT);
    pinMode(RIGHT_IN2, OUTPUT);
}

static void setMotorSpeed(int pin1, int pin2, float speed)
{
  if (speed > 0) {
    analogWrite(pin1, int(speed * 255));
    analogWrite(pin2, 0);
  } else {
    analogWrite(pin1, 0);
    analogWrite(pin2, int(-speed * 255));
  }
}

static void controlMotors(float Vx, float Vy)
{
  // Calculate the speed of each wheel
  float Va = Vx;
  float Vb = -0.5 * Vx + (sqrt(3) / 2.0) * Vy;
  float Vc = -0.5 * Vx - (sqrt(3) / 2.0) * Vy;

  // Normalize the speeds so that no motor exceeds 100% speed
  float maxVal = max(max(abs(Va), abs(Vb)), abs(Vc));
  if (maxVal > 1.0) {
    Va /= maxVal;
    Vb /= maxVal;
    Vc /= maxVal;
  }

  // Set motor speeds
  setMotorSpeed(LEFT_IN1, LEFT_IN2, Va);
  setMotorSpeed(MIDDLE_IN1, MIDDLE_IN2, Vb);
  setMotorSpeed(RIGHT_IN1, RIGHT_IN2, Vc);
}

static void stopAllMotors()
{
  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, LOW);
  digitalWrite(MIDDLE_IN1, LOW);
  digitalWrite(MIDDLE_IN2, LOW);
  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, LOW);
}