//EndlessSynth, generating square wave using timer interrupts.

//Ports in Arduino https://www.arduino.cc/en/Reference/PortManipulation
//Pins in Arduino Mega https://www.arduino.cc/en/Hacking/PinMapping2560

//Device: Arduino Mega (Note: Not Uno!)
//I use style of writing by using very basic Arduino functions,
//in order code will be more readable by beginners.

//Commutation:
const byte pin_buz = 2;

//Note: Monitor port baud must be set to 500000!

const long int audio_sample_rate = 44100;

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Square Wave Interrupt Test 1.1 for Arduino Mega");

  Serial.print("Buzzer pin: "); Serial.println(pin_buz);

  //Set port/pin mode for Arduino Mega for buzzer
  //https://www.arduino.cc/en/Hacking/PinMapping2560
  //pin 2 is 4 bit of Port E (in scheme PE2)
  DDRE = B00010000;    //pinMode(2,OUTPUT);

  

  //Set timer2 interrupt at "audio_sample_rate"
  int compare_match = 16000000 / (8*audio_sample_rate); //for timers0 and 2 must be 1..255, and for timer1 1..65535
  //compare_match = ...-1; ~44400 Hz.
  //compare_match = ...; ~43480 Hz.
  Serial.print("Interrupt compare match: "); Serial.println(compare_match);

  cli();    //stop interrupts
  TCCR2A = 0;   //set entire TCCR2A register to 0
  TCCR2B = 0;   //same for TCCR2B
  TCNT2  = 0;   //initialize counter value to 0
  OCR2A = compare_match; //set compare match register for "audio_sample_rate" increments
  TCCR2A |= (1 << WGM21); //turn on CTC mode
  TCCR2B |= (1 << CS21);    //Set CS21 bit for 8 prescaler
  TIMSK2 |= (1 << OCIE2A); //enable timer compare interrupt
  sei();    //start interrupts
}


//----------------------------------------------------------
int freq = 440; //frequency of sound

long int period_samples = audio_sample_rate / freq;
long int period_samples2 = period_samples/2;
//----------------------------------------------------------
//process timer interrupt event
long int t = 0;

long int phase = 0;

ISR(TIMER2_COMPA_vect) {
  t++;

  //I noted that here should be no "/" or "%" operations,
  //because timer is halted. As a workaround - use lower frame rate

  phase++;
  if (phase<period_samples2) {
    PORTE=B00010000; //buzzer ON
  }
  else {    
    PORTE=B00000000;  //buzzer OFF;
  }
  if (phase>=period_samples) phase = 0;
}

//----------------------------------------------------------
void loop(){
  
  long int time = millis();
  Serial.print("time: ");
  Serial.print(time);
  Serial.print("\t");
  Serial.print("counter: ");
  Serial.print(t);
  Serial.print("\trate: ");
  Serial.println(double(t) / (time/1000.0));

  delay(100);
}




//---------------------------------------------------------------
