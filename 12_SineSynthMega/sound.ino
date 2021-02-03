//Playing polyphonical sine wave using timer interrupt, for Mega.
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




//frequencies of sounds
//0 means note off
long int freq1 = 0;
long int freq2 = 0;
long int freq3 = 0;
long int freq4 = 0;

//---------------------------------------------------------------
//Set playing frequencies
//0 means note off
void set_freqs(int freq1_, int freq2_, int freq3_, int freq4_) {
  freq1 = freq1_;
  freq2 = freq2_;
  freq3 = freq3_;
  freq4 = freq4_;  
}

//---------------------------------------------------------------
//Set playing notes
//-1 means note off
void set_notes(char midi_note1, char midi_note2, char midi_note3, char midi_note4) {
  Serial.print("notes "); Serial.print(int(midi_note1)); 
  Serial.print(" "); Serial.print(int(midi_note2)); 
  Serial.print(" "); Serial.print(int(midi_note3)); 
  Serial.print(" "); Serial.println(int(midi_note4)); 
  
  freq1 = m_to_f(midi_note1);
  freq2 = m_to_f(midi_note2);
  freq3 = m_to_f(midi_note3);
  freq4 = m_to_f(midi_note4); 
}

//---------------------------------------------------------------
void sound_setup() {
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
//TODO put to const memory - print to console and after copy here
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
//process timer interrupt event
//sound generator
long int t = 0;

long int phase = 0;
int sound_value = 0;
const int diff_step = 127 * POLYPHONY; //step of diffusion subtraction - 1..127, kind of threshold for sound

//We should make this function as fast as possible, and trying to omit "/" and "%" operations
void timer_interrupt() {
  t++;

  //phase - is changed audio_sample_rate times per second
  //wave_n - length of wavetable
  //freq - desired frequency
  //ph - must go freq times 0..wave_n-1
  if (freq1) sound_value += wave_table[(((long long)phase * freq1 * wave_n) >> shift_audio) % wave_n];
#if POLYPHONY>=2
  if (freq2) sound_value += wave_table[(((long long)phase * freq2 * wave_n) >> shift_audio) % wave_n];
#endif
#if POLYPHONY>=3
  if (freq3) sound_value += wave_table[(((long long)phase * freq3 * wave_n) >> shift_audio) % wave_n];
#endif
#if POLYPHONY>=4
  if (freq4) sound_value += wave_table[(((long long)phase * freq4 * wave_n) >> shift_audio) % wave_n];
#endif

  phase++;

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
unsigned long int sound_time = 0;

void sound_loop() {
  unsigned long time = millis();
  if (time > sound_time + 200) {
    sound_time = time;
    long int time = millis();
    //Serial.print("time: ");
    //Serial.print(time);
    //Serial.print("\t");
    //Serial.print("counter: ");
    //Serial.print(t);
    
    //Serial.print("\trate: ");
    //Serial.println(double(t) / (time / 1000.0));


    /*
    static int note1 = 69 - 24; //midi bass note A
    static int note2 = note1 + 12 + 3; //second note
    static int note3 = note1 + 12 + 7; //second note
    static int note4 = note1 + 12 + 12; //second note
    freq1 = m_to_f(note1);
    freq2 = m_to_f(note2);
    freq3 = m_to_f(note1);
    freq4 = m_to_f(note2);
    //Serial.print("notes "); Serial.print(note1);
    //Serial.print(" "); Serial.println(note2);

    note1++;
    note2++;
    note3++;
    note4++;
    */
  }
}




//---------------------------------------------------------------
