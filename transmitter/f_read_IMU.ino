//--------Read IMU values--------
int read_IMU(){                   // replace with void if error
  //--------Read accelerometer values--------
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);               // register ACCEL_XOUT_H
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // read 6 registers then release, 2 axis per register (HIGH/LOW bits)
  AccX = (Wire.read() << 8 | Wire.read()) / 4096.0; // for 8g, LSB sensitivity = 4096 LSB/g 
  AccY = (Wire.read() << 8 | Wire.read()) / 4096.0;
  AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0;
  
  //--------Calculate acceleration angles--------
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) + 0.5; // accErrorX ~(-0.5)
  //accAngleX = (atan(AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 0.5;
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 3.15; // accErrorY ~(-3.15)
  //accAngleY = (atan(AccY / sqrt(pow(AccX, 2) + pow(AxxZ, 2))) * 180 / PI + 3.15;
  //accAngleZ = (atan(sqrt(pow(AccX, 2) + pow(AccY, 2)) / AccZ) * 180 / PI); // accErrorZ ?
  

  //--------Read gyroscope values--------
  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (previousTime - currentTime) / 1000; 
  Wire.beginTransmission(MPU);
  Wire.write(0x43);             // register GYRO_XOUT_H
  Wire.endTransmission(false);  // restart transmission - do not release bus
  Wire.requestFrom(MPU, 4, true); // read 4 registers then release, each axis value is stored in 2 registers
  GyX = (Wire.read() << 8 | Wire.read()) / 32.8; // for 1000 dps, LSB sensitivity = 32.8 LSB/deg/s
  GyY = (Wire.read() << 8 | Wire.read()) / 32.8;  
  GyX = GyX + 2.72; // gyroErrorX ~(-2.72)
  GyY = GyY - 1.69; // gyroErrorY ~(1.69)
  gyAngleX = GyX * elapsedTime; // convert deg/s to deg
  gyAngleY = GyY * elapsedTime;

  // Complementary filter - combine accelerometer and gyro angle values 
  angleX = 0.98 * (angleX + gyAngleX) + 0.02 * accAngleX;
  angleY = 0.98 * (angleY + gyAngleY) + 0.02 * accAngleY;

  //--------Replace joystick values with angle values-------- 
  data.joyLPotX = map(angleX, -90, 90, 0, 255); // Map angle values to byte values
  data.joyLPotY = map(angleY, -90, 90, 0, 255);

  //Serial.print("AccX: "); Serial.println(AccX);
  //Serial.print("AccY: "); Serial.println(AccY);
  //Serial.print("GyX: "); Serial.println(GyX);
  //Serial.print("GyY: "); Serial.println(GyY);
  //delay(500);

  return 0;
}
