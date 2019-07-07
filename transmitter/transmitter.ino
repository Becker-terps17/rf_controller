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

//--------Define IMU--------
const int MPU = 0x68; //MPU I2C address

//--------Define variables--------
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
