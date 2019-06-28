//receives 4 int array of pot values and writes them to servo pins 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(7, 8); // CNS, CE
const byte address[6] = "00001";
Servo a;
//Servo b;
//Servo c;
//Servo d;
//Servo e;

///////////////////////////For Motors/////////////////////////

void setup() {
  Serial.begin(9600);
  a.attach(5);
  //b.attach(6);
  //c.attach(9);
  //d.attach(10);
  //e.attach(3);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); //sets radio as receiver
}

int angle_total[5];

void loop() {
  delay(5);
  radio.startListening();
  int i, j;
  if ( radio.available()) {
    int angle[5] = {0,0,0,0,0}; 
    while (radio.available()) {
      //int angle[5] = {0,0,0,0,0};
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
  }
}



/////////////////////////////For Leds///////////////////////////
/*
void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(3, LOW);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); //sets radio as receiver
}

void loop() {
  delay(5);
  radio.startListening();
  int i,j;
  if ( radio.available()) {
    while (radio.available()) {
      int angle[5] = {0,0,0,0,0};
      //int state[5] = {0,0,0,0,0};
      radio.read(&angle, sizeof(angle));
      for (i=0; i<5; i++) {
        if (angle[i]<45 || angle[i]>135) {
          angle[i] = 1;
        }
        else {
          angle[i] = 0;
        }
        Serial.println(angle[i]);
      }
      digitalWrite(5,angle[0]);
      digitalWrite(6,angle[1]);
      digitalWrite(9,angle[2]);
      digitalWrite(10,angle[3]);
      digitalWrite(3,angle[4]);
    }
    delay(500);
    radio.stopListening();
  }
}
*/
