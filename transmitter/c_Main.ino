//--------Main loop--------
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

  //--------Confirm values being sent--------
  //Serial.println(data.joyLPotX);
  //Serial.println(data.joyLPotY);
  //Serial.println(data.joyLButton);
  //Serial.println(data.LPot);
  //Serial.println("RPotX, RPotY, RBut");
  //Serial.print(data.joyRPotX); Serial.print(", ");
  //Serial.print(data.joyRPotY); Serial.print(", ");
  //Serial.println(data.joyRButton);
  //delay(500);

  //--------Replace joysticks with IMU if toggle 1 is on--------
  if (digitalRead(t1) == 0) {
    Serial.println("using IMU values");
    read_IMU();
  }

  //read_IMU();

  radio.write(&data, sizeof(Data_Packet));
  delay(15);
}
