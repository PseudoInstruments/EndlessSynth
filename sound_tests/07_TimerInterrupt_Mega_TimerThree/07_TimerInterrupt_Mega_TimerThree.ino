//EndlessSynth, test of using Arduino Mega timer interrupts
//Time interrupts Implemented using TimerThree library.
//Please install "TimerThree" library using Arduino Library Manager

//This program runs TIMER3 at 44100 Hz, increase counter at each interrupt,
//and prints real rate between calls and time.
//Result:
//for audio_sample_rate really works at ~45000 Hz

//Device: Arduino Mega (Note: Not Uno!)
//Note: Monitor port baud must be set to 500000!

#include <TimerThree.h>
const long int audio_sample_rate = 44100;

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Interrupt Test with TimerThree 1.1");

  Timer3.initialize(1000000 / audio_sample_rate);
  Timer3.attachInterrupt(timer_interrupt);
  
}

//----------------------------------------------------------
//process timer interrupt event
long int t = 0;

void timer_interrupt() {
  t++;
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
