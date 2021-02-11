//EndlessSynth, polyphony sine wave test using diffusion with thresholding method and decaying
//- it's required to supress "high tone sound" occuring for silence in case of simple diffusion.
//Sine wave sounds cleaner than "pure" diffusion

//It supports 2,3,4 notes - set POLYPHONY define below.
//After starting it plays increasing chord.
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

//number of notes to play simultaneously, 1..4
//#define POLYPHONY (2)  
//#define POLYPHONY (3)  
#define POLYPHONY (4)  

//---------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Polyphony sine wave, diffusion with thresholding and decaying, v. 1.1 for Arduino Mega");

  Serial.print("Buzzer pin: "); Serial.println(pin_buz);

  Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  //Wavetable
  make_wavetable();


  //Set port/pin mode for Arduino Mega for buzzer
  //Ports in Arduino https://www.arduino.cc/en/Reference/PortManipulation
  //Pins in Arduino Mega https://www.arduino.cc/en/Hacking/PinMapping2560
  //pin 2 is 4 bit of Port E (in scheme PE2)
  DDRE = B00010000;    //pinMode(2,OUTPUT);

  //test_notes();
  //Start interrupt for sound generation
  Timer3.initialize(1000000 / audio_sample_rate);
  Timer3.attachInterrupt(timer_interrupt);

  Serial.print("Polyphony: "); Serial.println(POLYPHONY);
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
//frequencies of sounds
long int freq1 = 440; 
long int freq2 = 550; 
long int freq3 = 660; 
long int freq4 = 770;



//----------------------------------------------------------
//1-bit sound diffusion

long int sound_value = 0;
const int diff_step = //127 * POLYPHONY; //step of diffusion subtraction - 1..127, kind of threshold for sound
  70; //127;    //just 127 - to make polyphony sounding more "phatty"

const int thresh_sound = 10; //;  must be diff_step / 2, but for diff_keep <= 50 can be lower

//const int diff_keep = 30; //50;  //decaying diffusion in percents 0..100, 0 - no diffusion, 1 - keep all diffusion
//not used here, just >> instead. Can use table for fastening

//----------------------------------------------------------
//process timer interrupt event
//sound generator
long int t = 0;



long int phase = 0;


//We should make this function as fast as possible, and trying to omit "/" and "%" operations
void timer_interrupt() {
  t++;


  sound_value = sound_value >> 2;  //decaying diffusion   //sound_value * diff_keep / 100;  
    
  //phase - is changed audio_sample_rate times per second
  //wave_n - length of wavetable
  //freq - desired frequency
  //ph - must go freq times 0..wave_n-1
  sound_value += wave_table[(((long long)phase * freq1 * wave_n) >> shift_audio) % wave_n]; 
#if POLYPHONY>=2
  sound_value += wave_table[(((long long)phase * freq2 * wave_n) >> shift_audio) % wave_n]; 
#endif
#if POLYPHONY>=3
  sound_value += wave_table[(((long long)phase * freq3 * wave_n) >> shift_audio) % wave_n]; 
#endif
#if POLYPHONY>=4
  sound_value += wave_table[(((long long)phase * freq4 * wave_n) >> shift_audio) % wave_n]; 
#endif

  phase++;

  //output audio sample to buzzer
  //NOTE: silence generates 1/0/1/0 sequence in this approach:
  /*if (sound_value >= 0) {
    PORTE = B00010000; //buzzer ON
    sound_value -= diff_step;       //diffusion propagation
  }
  else {
    PORTE = B00000000; //buzzer OFF;
    sound_value += diff_step;       //diffusion propagation
  }*/
  
  //On removing 1/0/1/0 silence sequence problem:
  //Idea to use threshold for hysteresis:
  //>thresh -> HIGH,
  //-thresh..thresh - do nothing
  //<-thresh -> LOW.

  if (sound_value >= thresh_sound) {
    PORTE = B00010000; //buzzer ON
    sound_value -= diff_step;       //diffusion propagation
  }
  else {
    if (sound_value <= -thresh_sound) {
      PORTE = B00000000; //buzzer OFF;
      sound_value += diff_step;       //diffusion propagation
    }
  }
}


//----------------------------------------------------------
void loop() {


  long int time = millis();
  //Serial.print("time: ");
  //Serial.print(time);
  //Serial.print("\t");
  //Serial.print("counter: ");
  //Serial.print(t);
  Serial.print("\trate: ");
  Serial.println(double(t) / (time / 1000.0));

  static int note1 = 69-24; //midi bass note A
  static int note2 = note1+12+3; //second note
  static int note3 = note1+12+7; //second note
  static int note4 = note1+12+12; //second note
  freq1 = m_to_f(note1);
  freq2 = m_to_f(note2);
  freq3 = m_to_f(note1);
  freq4 = m_to_f(note2);
  Serial.print("notes "); Serial.print(note1); 
  Serial.print(" "); Serial.println(note2);
  
  note1++;
  note2++;
  note3++;
  note4++;
  delay(500);

}




//---------------------------------------------------------------
