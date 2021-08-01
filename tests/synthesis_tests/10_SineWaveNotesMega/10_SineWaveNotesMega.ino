//EndlessSynth, generating sine wave by given midi note number using timer interrupts.
//It starts from 69-24 bass A note and chromatically goes up
//For notes>100 some arithmetic overflow arises, and generates some "interesting" noise
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

//use lower values if not enough computing power
const long int audio_sample_rate = //44100;
  //power of 2 to faster divide
  8192;  //2^13
  //4096;  //2^12  

const long shift_audio = 13;
    //12;


//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Sine Wave Generator with Timer, v. 1.1 for Arduino Mega");

  Serial.print("Buzzer pin: "); Serial.println(pin_buz);

  Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  //Wavetable
  make_wavetable();

  Timer3.initialize(1000000 / audio_sample_rate);
  Timer3.attachInterrupt(timer_interrupt);

  //Set port/pin mode for Arduino Mega for buzzer
  //Ports in Arduino https://www.arduino.cc/en/Reference/PortManipulation
  //Pins in Arduino Mega https://www.arduino.cc/en/Hacking/PinMapping2560
  //pin 2 is 4 bit of Port E (in scheme PE2)
  DDRE = B00010000;    //pinMode(2,OUTPUT);

  test_notes();
}


//---------------------------------------------------------------
//Sine wavetable
const int wave_n = 256;
char wave_table[wave_n];

void make_wavetable() {
  int volume = 100; //0..127
  for (int i = 0; i < wave_n; i++) {
    wave_table[i] = int(sin(i * 2 * PI / wave_n) * volume);  //-127..127
  }

  /*Serial.println("Wavetable");
  for (int i = 0; i < wave_n; i++) {
    Serial.print(int(wave_table[i])); Serial.print(" ");
    if (i > 0 && i % 20 == 0) Serial.println();
  }
  Serial.println();
  */
}


//----------------------------------------------------------
long int freq = 440; //frequency of sound
            //500;

long int period_samples = audio_sample_rate / freq;

void set_freq(int freq0) {
  freq = freq0;
  period_samples = audio_sample_rate / freq;
}

//----------------------------------------------------------
//process timer interrupt event
//sound generator
long int t = 0;

long int phase = 0;
int sound_value = 0;
const int diff_step = 127; //step of diffusion subtraction - 1..127, kind of threshold for sound


//We should make this function as fast as possible, and trying to omit "/" and "%" operations
void timer_interrupt() {
  t++;

  //phase - is changed audio_sample_rate times per second
  //wave_n - length of wavetable
  //freq - desired frequency
  //ph - must go freq times 0..wave_n-1
  //int ph = ((long long)phase * freq * wave_n) / audio_sample_rate) % wave_n;
  int ph = (((long long)phase * freq * wave_n) >> shift_audio) % wave_n; 
  phase++;

  sound_value += wave_table[ph]; 

  //output audio sample to buzzer
  if (sound_value >= 0) {
    PORTE = B00010000; //buzzer ON
    sound_value -= diff_step;       //diffusion propagation
  }
  else {
    PORTE = B00000000; //buzzer OFF;
    sound_value += diff_step;       //diffusion propagation
  }
  //if (phase >= period_samples) phase = 0;
}


//----------------------------------------------------------
void loop() {


  long int time = millis();
  //Serial.print("time: ");
  //Serial.print(time);
  //Serial.print("\t");
  //Serial.print("counter: ");
  //Serial.print(t);
  //Serial.print("\trate: ");
  //Serial.println(double(t) / (time / 1000.0));

  static int note = 69-24; //midi bass note A
  int freq = m_to_f(note);
  set_freq(freq);
  Serial.print("note "); Serial.print(note);
  Serial.print(", freq "); Serial.println(freq);
  
  note++;
  delay(200);

}




//---------------------------------------------------------------
