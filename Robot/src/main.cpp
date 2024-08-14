#include "main.h"

BluetoothSerial SerialBT;

void setup()
{
    Serial.begin(9600);
    SerialBT.begin("Robot"); // Bluetooth device name

    pinSetup();
}

void loop()
{
    if (SerialBT.available())
    {
        String data = SerialBT.readString();

        // Expected data format: "Vx,Vy,omega"
        int firstCommaIndex = data.indexOf(',');
        int secondCommaIndex = data.indexOf(',', firstCommaIndex + 1);

        float Vx = data.substring(0, firstCommaIndex).toFloat();
        float Vy = data.substring(firstCommaIndex + 1, secondCommaIndex).toFloat();
        float omega = data.substring(secondCommaIndex + 1).toFloat();
        // Serial.print("Vx = ");
        // Serial.print(Vx);
        // Serial.print(", Vy = ");
        // Serial.print(Vy);
        // Serial.print(", Omega = ");
        // Serial.println(omega);

        controlMotors(Vx, Vy, omega);
    }
}

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
    const float minDutyCycle = 0.27;
    if (speed > 0)
    {
        float adjusted_speed = max(speed, minDutyCycle);
        analogWrite(pin1, int(adjusted_speed * 255));
        analogWrite(pin2, 0);
    }
    else if (speed < 0)
    {
        float adjusted_speed = max(-speed, minDutyCycle);
        analogWrite(pin1, 0);
        analogWrite(pin2, int(-adjusted_speed * 255));
    }
    else
    {
      analogWrite(pin1, 0);
      analogWrite(pin2, 0);
    }
}

static void controlMotors(float Vx, float Vy, float omega)
{
    // Robot radius
    const float R = 0.1;

    // Calculate the speed of each wheel
    float Va = Vx - omega * R;
    float Vb = -0.5 * Vx + (sqrt(3) / 2.0) * Vy - omega * R;
    float Vc = -0.5 * Vx - (sqrt(3) / 2.0) * Vy - omega * R;

    // Normalize the speeds so that no motor exceeds 100% speed
    float maxVal = max(max(abs(Va), abs(Vb)), abs(Vc));
    if (maxVal > 1.0)
    {
        Va /= maxVal;
        Vb /= maxVal;
        Vc /= maxVal;
    }

    Serial.print("Va = ");
    Serial.print(Va);
    Serial.print(", Vb = ");
    Serial.print(Vb);
    Serial.print(", Vc = ");
    Serial.println(Vc);

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