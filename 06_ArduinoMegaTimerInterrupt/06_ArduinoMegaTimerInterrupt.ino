//EndlessSynth, test of using Arduino Mega timer interrupts

//This program runs TIMER2 at 44100 Hz, increase counter at each interrupt,
//and prints real rate between calls and time.
//Result: 
//compare_match = ...-1; ~44400 Hz.
//compare_match = ...; ~43480 Hz.


//Based on https://www.instructables.com/Arduino-Timer-Interrupts/

//Device: Arduino Mega (Note: Not Uno!)

//Note: Monitor port baud must be set to 500000!

const long int audio_sample_rate = 44100;

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Interrupt Test 1.1");

  int compare_match = 16000000 / (8*audio_sample_rate); //for timers0 and 2 must be 1..255, and for timer1 1..65535
  //compare_match = ...-1; ~44400 Hz.
  //compare_match = ...; ~43480 Hz.
  Serial.print("Interrupt compare match: "); Serial.println(compare_match);


  //set timer2 interrupt at "audio_sample_rate"
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
//process timer interrupt event
long int t = 0;

ISR(TIMER2_COMPA_vect) {
  t+=1;
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
