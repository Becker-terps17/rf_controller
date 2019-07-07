//--------Calculate IMU error--------
int calc_IMU_error() {
  /*
   * Use this to get error values for accelerometer and gyroscope tuning.
   * Note: Place IMU flat to get accurate values for correction factors
   */
   
  //--------Collect 200 sample acceleration values--------
  while (c < 200) {
    //Serial.println("Start acceleration error data collection");
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);             // register ACCEL_XOUT_H
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 4096.0; // for 8g, LSB sensitivity = 4096 LSB/g
    AccY = (Wire.read() << 8 | Wire.read()) / 4096.0;
    AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0;
    //--------Sum all angle calculations--------
    accErrorX = accErrorX + (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI);
    accErrorY = accErrorY + (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI);
    //accErrorZ = accErrorZ + (atan(sqrt(pow(AccX, 2) + pow(AccY, 2)) / AccZ) * 180 / PI);
    c++;
  }
  
  //--------Find average of acceleration angle values--------
  accErrorX = accErrorX / 200;
  accErrorY = accErrorY / 200;
  //accErrorZ = accErrorZ / 200;

  //--------Reset c--------
  c = 0;
  
  //--------Collect 200 sample gyroscope values
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 4, true);
    GyX = (Wire.read() << 8 | Wire.read()) / 32.8; // for 1000dps, LSB sensitivity = 32.8 LSB/deg/s
    GyY = (Wire.read() << 8 | Wire.read()) / 32.8;
    //--------Sum all angle calculations--------
    gyErrorX = gyErrorX + GyX;
    gyErrorY = gyErrorY + GyY;
    c++;
  }
  
  //--------Find average of gyroscope angle values--------
  gyErrorX = gyErrorX / 200;
  gyErrorY = gyErrorY / 200;
  
  //--------Print error values--------
  Serial.print("accErrorX: ");
  Serial.println(accErrorX);
  Serial.print("accErrorY: ");
  Serial.println(accErrorY);
  Serial.print("gyErrorX: ");
  Serial.println(gyErrorX);
  Serial.print("gyErrorY: ");
  Serial.println(gyErrorY);
 
}
