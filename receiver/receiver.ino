/* 
 * RF Receiver
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
#include <Servo.h>

//--------Define radio--------
RF24 radio(8, 7); // CE, CSN
const byte address[6] = "00001";

//--------Define servos--------
//Servo a;
//Servo b;
//Servo c;
//Servo d;
//Servo e;

//--------Define variables--------
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

//int angle_total[5];

//--------Define data packet--------
// Max size of structure is 32 bytes (buffer limit for RF module)
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

Data_Packet data; // create a variable with the above data structure

//--------Initial Setup--------
void setup() {
  //--------Initialize serial port--------
  Serial.begin(9600);

  //--------Attach servo objects--------
  //a.attach(5);
  //b.attach(6);
  //c.attach(9);
  //d.attach(10);
  //e.attach(3);

  //--------Initialize RF--------
  radio.begin();
  radio.openReadingPipe(0, address);
  //radio.setAutoAck(false); // auto-acknowledge packets - enabled by default
  //radio.setDataRate(RF24_250KBPS); // will fail for non-plus units
  radio.setPALevel(RF24_PA_MIN); // set power amplifer level - min is fine for close range
  radio.startListening(); //sets radio as receiver
  resetData();
}

void loop() {
  //--------Check for data--------
  int i, j;
  
  if (radio.available()) {
    int angle[5] = {0,0,0,0,0};
    radio.read(&data, sizeof(Data_Packet)); // read the whole data and store in 'data' structure
    lastReceiveTime = millis(); // time stamp for data received
  }
  
  //--------Check for incoming data and module connection--------
  currentTime = millis();
  if (currentTime - lastReceiveTime > 1000) {
    resetData(); // reset data if connection is lost for 1 sec 
  }

  //--------Print data--------
  Serial.print("joyLPotX: ");
  Serial.print(data.joyLPotX);
  Serial.print("; joyLPotY: ");
  Serial.print(data.joyLPotY);
  Serial.print("; joyLButton: ");
  Serial.print(data.joyLButton);
  Serial.print("; joyRPotX: ");
  Serial.println(data.joyRPotX);

  delay(500);
}

  /*
    while (radio.available()) {
      radio.read(&angle, sizeof(angle));
      for (i=0; i<5; i++){
        
        //Eliminate deadzone readings in joystick 
        if (angle[i] > 1 || angle[i] < -1){ 
          angle[i] = angle[i];
        }
        else {
          angle[i] = 0; 
        }
        
        angle_total[i] = angle[i] + angle_total[i]; //need to add bounds to this or it will grow forever
        //Serial.println(angle[i]);
        Serial.println(angle_total[i]); 
      }

      //Serial.println(angle_total[0]); 
      
      a.write(angle_total[0]);
      //b.write(angle[1]);
      //c.write(angle[2]);
      //d.write(angle[3]); // change back to 3 when using all motors
      //e.write(angle[4]); 
    }
    delay(20);
    radio.stopListening();
    */


//--------Reset data when connection is lost--------
int resetData() {
  // reset to initial default values
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
}
