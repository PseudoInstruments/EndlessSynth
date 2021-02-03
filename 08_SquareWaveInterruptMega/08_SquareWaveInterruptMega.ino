//EndlessSynth, generating square wave using timer interrupts.
//
//Aditional libraries:
//- install "TimerThree" using Library Manager
//Device: Arduino Mega (Note: Not Uno!)
//Commutation:
//- buzzer at pin 2
const byte pin_buz = 2;

//Monitor port:
//- baud must be set to 500000!
//---------------------------------------------------------------

#include <TimerThree.h>
const long int audio_sample_rate = 44100;

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Square Wave Interrupt Test 1.1 for Arduino Mega");

  Serial.print("Buzzer pin: "); Serial.println(pin_buz);

  Timer3.initialize(1000000 / audio_sample_rate);
  Timer3.attachInterrupt(timer_interrupt);

  //Set port/pin mode for Arduino Mega for buzzer
  //Ports in Arduino https://www.arduino.cc/en/Reference/PortManipulation
  //Pins in Arduino Mega https://www.arduino.cc/en/Hacking/PinMapping2560
  //pin 2 is 4 bit of Port E (in scheme PE2)
  DDRE = B00010000;    //pinMode(2,OUTPUT);


}


//----------------------------------------------------------
int freq = 440; //frequency of sound

long int period_samples = audio_sample_rate / freq;
long int period_samples2 = period_samples / 2; //for optimization interrupt code
//----------------------------------------------------------
//process timer interrupt event
long int t = 0;

long int phase = 0;

void timer_interrupt() {
  t++;

  phase++;

  //NOTE: it's important to make interrupt code fast - for example,
  //if we will use "period_samples/2" instead of precomputed period_samples2,
  //the tone will be lower, and printing will be significantly slower
  //Of course, we can use "<<" for faster work, but here we just checking
  //overall performance.
  //Also, I checked that we may reduce audio_sample_rate, for example, to 10000,
  //and then use "/2".
  if (phase < period_samples2) {
    PORTE = B00010000; //buzzer ON
  }
  else {
    PORTE = B00000000; //buzzer OFF;
  }
  if (phase >= period_samples) phase = 0;
}

//----------------------------------------------------------
void loop() {

  long int time = millis();
  Serial.print("time: ");
  Serial.print(time);
  Serial.print("\t");
  Serial.print("counter: ");
  Serial.print(t);
  Serial.print("\trate: ");
  Serial.println(double(t) / (time / 1000.0));

  delay(100);
}




//---------------------------------------------------------------
