//transmits a 4 int array over RF, {x1, y1, x2, y2}
//
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CNS, CE
const byte address[6] = "00001";
void setup() {
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN); //sets power amplifer level, min is fine for close range
  radio.stopListening(); //sets radio as transmitter
}

void loop() {
  int i;
  
//Original with all analog inputs and absolute mapping
  /*int Values[5] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4)};
  //int angleValue[5];
  //for (i=0; i<5; i++){
    //angleValue[i] = map(Values[i], 0, 1023, 0, 180);
  */

  
  int Values[6] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), digitalRead(3), digitalRead(4)};
  int angleValue[5]; //only send one value for both digital inputs
  for (i=0; i<4; i++){
    angleValue[i] = map(Values[i], 0, 1023, -7, 7); //corresponds to relative degree change 
  }
// joystick reads LOW when pressed
  if((Values[4] == 0) && (Values[5] == 1)){ //change back to 4 & 5 respectively****
    angleValue[4] = 5; //close claw 5 degrees
  }
  else if((Values[4] == 1) && (Values[5] == 0)){
    angleValue[4] = -5; //open claw 5 degrees
  }
  else{
    angleValue[4] = 0; //don't move claw if both are pressed or neither are pressed
  }

  radio.write(&angleValue, sizeof(angleValue));
  delay(20);
}
