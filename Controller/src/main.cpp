#include "main.h"

BluetoothSerial SerialBT;
Adafruit_MPU6050 mpu;

bool connected = false;

void setup()
{
    Serial.begin(9600);
    SerialBT.begin("Controller", true); // Bluetooth device name

    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }

    Serial.println("MPU6050 connected successfully!");

    // Attempt to connect to the robot
    Serial.println("Attempting to connect to robot...");
    if (SerialBT.connect("Robot"))
    {
        Serial.println("Connected to robot!");
        connected = true;
    }
    else
    {
        Serial.println("Failed to connect to robot.");
    }

    // Set accelerometer range to +/- 4G
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    // Set gyroscope range to +/- 250 deg/s
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    // Set filter bandwidth to 21 Hz
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop()
{
    if (connected)
    {
        float Vx, Vy, omega;
        readIMUData(Vx, Vy, omega);

        // Send the movement vector as "Vx,Vy,omega"
        String data = String(Vx) + "," + String(Vy) + "," + String(omega);
        SerialBT.println(data);

        // Print the data being sent for debugging
        Serial.print("Sending data: ");
        Serial.println(data);

        delay(50); // Adjust as needed for smoother control (20ms = 50Hz)
    }
    else
    {
        // Retry connection if disconnected
        Serial.println("Attempting to reconnect to robot...");
        if (SerialBT.connect("Robot"))
        {
            Serial.println("Reconnected to robot!");
            connected = true;
        }
        else
        {
            Serial.println("Failed to reconnect to robot.");
            delay(1000); // Wait before retrying
        }
    }
}

static void readIMUData(float &Vx, float &Vy, float &omega)
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Calculate pitch and roll from accelerometer data
    float pitch = atan2(a.acceleration.x, a.acceleration.z) * 180.0 / PI;
    float roll = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;

    // Calculate yaw (rotation around z-axis) using gyroscope data
    omega = g.gyro.z; // Angular velocity around the z-axis (in radians/sec)

    // Normalize pitch and roll to Vx and Vy
    Vx = constrain(roll / 20.0, -1.0, 1.0);
    Vy = constrain(pitch / 20.0, -1.0, 1.0);

    // Print the pitch, roll, Vx, Vy, and omega values for debugging
    Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print(", Roll: ");
    Serial.print(roll);
    Serial.print(", Vx: ");
    Serial.print(Vx);
    Serial.print(", Vy: ");
    Serial.print(Vy);
    Serial.print(", Omega: ");
    Serial.println(omega);
    delay(2000);
}