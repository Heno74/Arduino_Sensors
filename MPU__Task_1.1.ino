/*
Author : Mohamed Hanii
Date : 20/ 3 / 2021
Sensor :  MPU6050 
Data sheet : https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
version : 1.1
ref : https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
*/
#include <Wire.h>
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;
void setup() {
  //Accelerometer setup:
    Wire.beginTransmission(MPU);
    Wire.write(0x1C);// ACCEL_CONFIG
    Wire.write(0x10);// set full scale rage ± 8g 
 //Gyro setup:
    Wire.beginTransmission(MPU);
    Wire.write(0x1B);// ACCEL_CONFIG
    Wire.write(0x10);// set full scale rage ± 1000 °/
}

void loop() {
// Reading Accelerometer Data &  Calculating Roll and Pitch from the accelerometer data :
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);// start with first bit for x in 8bit regigster
    Wire.endTransmission(false); // false will send a restart, keeping the connection active.
    Wire.requestFrom(MPU, 6, true); // to make master read six readings 2 for each axis   
    AccX = (Wire.read() << 8 | Wire.read()) / 4096.0; // X-axis value   number after / depends on range you have choose
    AccY = (Wire.read() << 8 | Wire.read()) / 4096.0; // Y-axis value
    AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0; // Z-axis value
    accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58; 
    accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58; // avoidd wrong measurement
// Reading  Gyro Data &  Calculating Roll and Pitch from the accelerometer data :
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // unit : seconds
  Wire.beginTransmission(MPU); // like acellemtor
  Wire.write(0x43); 
  Wire.endTransmission(false); // false will send a restart, keeping the connection active.
  Wire.requestFrom(MPU, 6, true); // to make master read six readings 2 for each axis   
  GyroX = (Wire.read() << 8 | Wire.read()) / 32.8; // number after / depends on range you have choose
  GyroY = (Wire.read() << 8 | Wire.read()) / 32.8;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 32.8;
  // Correct the outputs with the calculated error values
  GyroX = GyroX + 0.56; // GyroErrorX ~(-0.56)
  GyroY = GyroY - 2; // GyroErrorY ~(2)
  GyroZ = GyroZ + 0.79; // GyroErrorZ ~ (-0.8)
  // change uints from degree/s to degree
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  yaw =  yaw + GyroZ * elapsedTime;
  //  Complementary filter : combine acceleromter and gyro angle values :
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  Serial.print("roll : ");
  Serial.print(roll);
  Serial.print(" / ");
  Serial.print("pitch : ");
  Serial.print(pitch);
  Serial.print(" / ");
  Serial.println("yaw : ");
  Serial.println(yaw);

}
