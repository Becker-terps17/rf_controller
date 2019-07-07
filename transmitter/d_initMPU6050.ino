//--------Initalize IMU--------
void init_MPU6050() {
  //Wire.begin();                 // should be called only once
  Wire.beginTransmission(MPU); 
  Wire.write(0x6B);             // talk to PWR_MGMT_1 register 6B
  //Wire.write(0x00);             // make reset - Set CLKSEL to internal 8MHz oscillator
  Wire.write(0);                // wake up MPU
  Wire.endTransmission(true);
  // Configure Accelerometer 
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);             // talk to ACCEL_CONFIG register 1C
  Wire.write(0x10);             // set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  // Configure Gyroscope
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);             // talk to GYRO_CONFIG register 1B
  Wire.write(0x10);             // set the register bits as 00010000 (1000dps full scale) 
  Wire.endTransmission(true);
  
}
