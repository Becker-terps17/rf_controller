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
  Wire.begin();               // join I2C bus (should be called only once)
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
  radio.setAutoAck(false); // auto-acknowledge packets - enabled by default 
  radio.setDataRate(RF24_250KBPS); // will fail for non-plus units 
  radio.setPALevel(RF24_PA_MIN); // sets power amplifer level, min is fine for close range
  radio.stopListening(); // sets radio as transmitter
}
