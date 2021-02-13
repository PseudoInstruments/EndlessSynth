//Playing polyphonical sine wave using timer interrupt, for Mega.
//---------------------------------------------------------------

#include <TimerThree.h>

//---------------------------------------------------------------
//Sound Engine
//---------------------------------------------------------------
//Audio sample rate - main parameter of sound engine.
//It's the frequency of sound pulses.
//Use lower values if not enough computing power; currently we use power of 2 to faster divide

const long int audio_sample_rates[2] = //{2048,8192}; 
                                      {4096, 8192};
const byte audio_sample_rate_shifters[2] = //{11,13}; 
                                      {12, 13};   //log_2 of sample rate, 4096=2^12 and so on

long int audio_sample_rate = 2048; //4096;  
byte audio_sample_rate_shifter = 11; //12; 


byte audio_sample_rate_index = 255; //0 - 4096, 1 - 8192, 255 - not set

//change audio sample rate
void set_audio_sample_rate_index(byte index) {
  if (audio_sample_rate_index!=index) {
    audio_sample_rate_index = index;
    audio_sample_rate = audio_sample_rates[index];
    audio_sample_rate_shifter = audio_sample_rate_shifters[index];
    Timer3.stop();
    Timer3.detachInterrupt();
    Timer3.initialize(1000000 / audio_sample_rate);
    Timer3.attachInterrupt(timer_interrupt);
    Timer3.start();

    Serial.print("sample rate: "); Serial.println(audio_sample_rate);
  }
}


//---------------------------------------------------------------
//Synthesis
//---------------------------------------------------------------
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
//-1 means note off, that os freqi=0
void set_notes(char midi_note1, char midi_note2, char midi_note3, char midi_note4, char base_note) {
  Serial.print("notes "); Serial.print(int(midi_note1));
  Serial.print(" "); Serial.print(int(midi_note2));
  Serial.print(" "); Serial.print(int(midi_note3));
  //Serial.print(" "); Serial.print(int(midi_note4));

  freq1 = (midi_note1 != -1) ? m_to_f_int(midi_note1 + base_note) : 0;
  freq2 = (midi_note2 != -1) ? m_to_f_int(midi_note2 + base_note) : 0;
  freq3 = (midi_note3 != -1) ? m_to_f_int(midi_note3 + base_note) : 0;
#if POLUPHONY>=4
  freq4 = (midi_note4 != -1) ? m_to_f_int(midi_note4 + base_note) : 0;
#endif

  Serial.print("   freq "); Serial.print(freq1);
  Serial.print(" "); Serial.print(freq2);
  Serial.print(" "); Serial.print(freq3);
  //Serial.print(" "); Serial.print(int(freq4));
  Serial.println();
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
  set_audio_sample_rate_index(1);  //Timer3.initialize(1000000 / audio_sample_rate);

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
//1-bit sound diffusion parameters

const int min_diff_step = 1;    //Parameter for slider
const int max_diff_step = 256;    //Parameter for slider
int diff_step = //127 * POLYPHONY; //step of diffusion subtraction - 1..127, kind of threshold for sound
  90; //127;    //just 127 - to make polyphony sounding more "phatty"


int diff_keep = 64; //decaying diffusion in percents 0..256, 0 - no diffusion, 256 - keep all diffusion
const int diff_keep_denom = 256;

//threshold for switching buzzer, must be so that not to allow "silence beep"
int thresh_sound = 20;
//diff_step / 2; //but for diff_keep <= 50 can be lower

//Formula, but not tested for this sound
//const int thresh_sound = ((long int) diff_step) * diff_keep / (2*diff_keep_denom) + 1;

//Variable of sound output
long int sound_value = 0; //diffusion value


//----------------------------------------------------------
//Updating parameters of algorithm
//  alg_step10bit 0..1023 - kind of "level of sound generation", the higher - the more dithering
//  alg_diff10bit 0..1023 - memory of dithering, the higher - the more harshness
void sound_set_1bitparams(int alg_step10bit, int alg_diff10bit) {
  diff_step = ((long int)alg_step10bit) * (max_diff_step - min_diff_step) / 1024 + min_diff_step;
  diff_keep = ((long int)alg_diff10bit) * diff_keep_denom / 1024;

  //Formula for threshold
  thresh_sound = ((long int) diff_step) * diff_keep / (diff_keep_denom) + 1; //I expect to use 2*diff_keep_denom, but beep occurs:)


  //Debug print - uncomment to print sliders values
  //Serial.print("diff_step "); Serial.print(diff_step);
  //Serial.print(", diff_keep float "); Serial.print(float(diff_keep)/diff_keep_denom);
  //Serial.print(", thresh_sound "); Serial.print(thresh_sound);
  //Serial.println();

}

//----------------------------------------------------------
//process timer interrupt event
//sound generator
long int phase = 0;

//We should make this function as fast as possible, and trying to omit "/" and "%" operations
void timer_interrupt() {
  //decay accumulated diffusion, because in opposite case zero values will give constant high-tone
  sound_value = (sound_value * diff_keep) >> 8;    // >> 8 means / diff_keep_denom //sound_value >> 2;

  //----------------------------------
  if (mic_button) {
    //Microphone
    sound_value += (analogRead(A0) - 512) >> 2; //0..1023 -> -512..511 -> -128..127   - we do so because mic is quiet...
  }
  //----------------------------------
  else {
    //Sine wave
    //phase - is changed audio_sample_rate times per second
    //wave_n - length of wavetable
    //freq - desired frequency
    //ph - must go freq times 0..wave_n-1
    if (freq1) sound_value += wave_table[(((long long)phase * freq1 * wave_n) >> audio_sample_rate_shifter) % wave_n];
    if (freq2) sound_value += wave_table[(((long long)phase * freq2 * wave_n) >> audio_sample_rate_shifter) % wave_n];
    if (freq3) sound_value += wave_table[(((long long)phase * freq3 * wave_n) >> audio_sample_rate_shifter) % wave_n];
    //  if (freq4) sound_value += wave_table[(((long long)phase * freq4 * wave_n) >> shift_audio) % wave_n];
  }
  //----------------------------------

  phase++;

  //output audio sample to buzzer using dithering algorithm threshold+decay
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
