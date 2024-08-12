#include "main.h"

BluetoothSerial SerialBT;

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("Robot"); // Bluetooth device name

  pinSetup();
}

void loop()
{
  if (SerialBT.available())
  {
    String data = SerialBT.readString();
    float Vx = data.substring(0, data.indexOf(',')).toFloat();
    float Vy = data.substring(data.indexOf(',') + 1).toFloat();

    controlMotors(Vx, Vy);
  }
}