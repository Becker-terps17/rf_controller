/* 
 * RF Controller Transmitter
 * By: Alex Smith
 * 
 * Inspired and guided By: Dejan Nedelkovski's Arduino based RC Transmitter, www.HowToMechatronics.com
 * Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
 * 
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

//--------Define radio--------
RF24 radio(5, 6); // (CE, CSN)
const byte address[6] = "00001";

//--------Define variables--------
const int MPU = 0x68; //MPU I2C address
float AccX, AccY, AccZ;
float GyX, GyY, GyZ;
float accAngleX, accAngleY, gyAngleX, gyAngleY;
float angleX, angleY;
//float accAngleZ, gyAngleZ, accErrorZ, gyErrorZ;
float accErrorX, accErrorY, gyErrorX, gyErrorY;
float elapsedTime, currentTime, previousTime;
int c = 0;

//--------Digital Inputs-------- 
#define jLBut 0
#define jRBut 1
#define b1 8
#define b2 9
#define b3 2
#define b4 3
#define t1 7
#define t2 4


//--------Define Data Packet-------- 
// Data to be sent via nRF24L01
// Max size of structure is 32 bytes (buffer limit for RF modulde) 
struct Data_Packet {
  byte joyLPotX;
  byte joyLPotY;
  byte joyLButton;
  byte joyRPotX;
  byte joyRPotY;
  byte joyRButton;
  byte LPot;
  byte RPot;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
  byte toggle1;
  byte toggle2;
};

Data_Packet data; //Create a variable with the above structure 

//--------Initial Setup --------
void setup() {
  //--------Initalize serial port--------
  Serial.begin(9600);

  //--------Initialize digital pins--------
  pinMode(jLBut, INPUT_PULLUP);       // with pullup resistor digital pins will read 0 when button is pressed
  pinMode(jRBut, INPUT_PULLUP);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(b4, INPUT_PULLUP);
  pinMode(t1, INPUT_PULLUP);
  pinMode(t2, INPUT_PULLUP);

  //--------Initialize IMU--------
  init_MPU6050();

  //--------Initialize Resting Values--------
  data.joyLPotX = 127;
  data.joyLPotY = 127;
  data.joyRPotX = 127;
  data.joyRPotY = 127;
  data.LPot = 127;
  data.RPot = 127;

  data.joyLButton = 1;
  data.joyRButton = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
  data.toggle1 = 1;
  data.toggle2 = 1;

  //--------Get IMU error--------
  //calc_IMU_error();

  //--------Initialize RF--------
  radio.begin();
  radio.openWritingPipe(address);
  //radio.setAutoAck(false); // auto-acknowledge packets - enabled by default 
  //radio.setDataRate(RF24_250KBPS); // will fail for non-plus units 
  radio.setPALevel(RF24_PA_MIN); // sets power amplifer level, min is fine for close range
  radio.stopListening(); // sets radio as transmitter
}

void loop() {
  //--------Map analog inputs to byte value--------
  data.joyLPotX = map(analogRead(A0), 0, 1023, 0, 255); 
  data.joyLPotY = map(analogRead(A1), 0, 1023, 0, 255);
  data.joyRPotX = map(analogRead(A2), 0, 1023, 0, 255);
  data.joyRPotY = map(analogRead(A3), 0, 1023, 0, 255);
  data.LPot = map(analogRead(A7), 0, 1023, 0, 255);
  data.RPot = map(analogRead(A6), 0, 1023, 0, 255);
  
  //--------Read digital inputs--------
  data.joyLButton = digitalRead(jLBut);
  data.joyRButton = digitalRead(jRBut);
  data.button1 = digitalRead(b1);
  data.button2 = digitalRead(b2);
  data.button3 = digitalRead(b3);
  data.button4 = digitalRead(b4);
  data.toggle1 = digitalRead(t1);
  data.toggle2 = digitalRead(t2);

  //--------Replace joysticks with IMU if toggle 1 is on--------
  if (digitalRead(t1) == 0) {
    read_IMU();
  }

  radio.write(&data, sizeof(Data_Packet));
  delay(15);
}

//--------Initalize IMU--------
void init_MPU6050() {
  Wire.begin();                 // should be called only once
  Wire.beginTransmission(MPU); 
  Wire.write(0x6B);             // talk to PWR_MGMT_1 register 6B
  Wire.write(0x00);             // make reset - Set CLKSEL to internal 8MHz oscillator
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

//--------Calculate IMU error--------
int calc_IMU_error() {
  /*
   * Use this to get error values for accelerometer and gyroscope tuning.
   * Note: Place IMU flat to get accurate values for correction factors
   */
   
  //--------Collect 200 sample acceleration values--------
  while (c < 200) {
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
  //Serial.print("accErrorX: ", accErrorX, "\n");
  //Serial.print("accErrorY: ", accErrorY, "\n");
  //Serial.print("gyErrorX: ", gyErrorX, "\n");
  //Serial.print("gyErrorY: ", gyErrorY, "\n");
 
}




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
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) + 1.15; // accErrorX ~(-1.15)
  //accAngleX = (atan(AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.15;
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - 0.52; // accErrorY ~(0.5)
  //accAngleY = (atan(AccY / sqrt(pow(AccX, 2) + pow(AxxZ, 2))) * 180 / PI - 0.52;
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
  GyX = GyX + 1.85; // gyroErrorX ~(-1.85)
  GyY = GyY - 0.15; // gyroErrorY ~(0.15)
  gyAngleX = GyX * elapsedTime; // convert deg/s to deg
  gyAngleY = GyY * elapsedTime;

  // Complementary filter - combine accelerometer and gyro angle values 
  angleX = 0.98 * (angleX + gyAngleX) + 0.02 * accAngleX;
  angleY = 0.98 * (angleY + gyAngleY) + 0.02 * accAngleY;

  //--------Replace joystick values with angle values-------- 
  data.joyLPotX = map(angleX, -90, 90, 0, 255); // Map angle values to byte values
  data.joyLPotY = map(angleY, -90, 90, 0, 255);

  return 0;
}
