//EndlessSynth, test of using Arduino ports for faster pin control

//Ports in Arduino https://www.arduino.cc/en/Reference/PortManipulation
//Pins in Arduino Mega https://www.arduino.cc/en/Hacking/PinMapping2560

//Device: Arduino Mega (Note: Not Uno!)
//I use style of writing by using very basic Arduino functions,
//in order code will be more readable by beginners.

//Commutation:
const byte pin_buz = 2;

//Note: Monitor port baud must be set to 500000!

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Port Test 1.1");

  Serial.print("Buzzer pin: "); Serial.println(pin_buz);

  //set port/pin mode for Arduino Mega
  //https://www.arduino.cc/en/Hacking/PinMapping2560
  //pin 2 is 4 bit of Port E (in scheme PE2)
  DDRE = B00010000;    //pinMode(2,OUTPUT);
}


void loop(){
  //square wave on pin 2
  PORTE=B00010000; //digitalWrite(pin_buz,HIGH);
  delayMicroseconds(1000000/440);
  
  PORTE=B00000000;  //digitalWrite(pin_buz,LOW);
  delayMicroseconds(1000000/440);
}



//---------------------------------------------------------------
