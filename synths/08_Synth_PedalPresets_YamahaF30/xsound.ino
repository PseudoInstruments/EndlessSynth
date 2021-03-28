//Sound engine
//Playing polyphonical sine wave using timer interrupt, for Mega.

//Note: Engine uses ports for fater output data to pins,
//so can affect other pins on PORTE, see end of the file for list 
//---------------------------------------------------------------

#include <TimerThree.h>

//---------------------------------------------------------------
//Sound Engine
//---------------------------------------------------------------
//Audio sample rate - main parameter of sound engine.
//It's the frequency of sound pulses.
//Use lower values if not enough computing power; currently we use power of 2 to faster divide

const long audio_sample_rate0 = 2000; 
const long audio_sample_rate1 = 8000;    //More frame rate gives overflow and three notes freeze synth
const long audio_sample_rate_step = 20; //"big" difference when change frame rate

//const long int audio_sample_rates[2] = {2000,4000}; //{2048,8192};
//{4096, 8192};
//const byte audio_sample_rate_shifters[2] = //{11,13};
//                                      {12, 13};   //log_2 of sample rate, 4096=2^12 and so on

long int audio_sample_rate = 0; //2048; //4096;
//byte audio_sample_rate_shifter = 11; //12;


//byte audio_sample_rate_index = 255; //0 - 4096, 1 - 8192, 255 - not set

//change audio sample rate
/*void set_audio_sample_rate_index(byte index) {
  if (audio_sample_rate_index!=index) {
    audio_sample_rate_index = index;
    audio_sample_rate = audio_sample_rates[index];
    //audio_sample_rate_shifter = audio_sample_rate_shifters[index];
    Timer3.stop();
    Timer3.detachInterrupt();
    Timer3.initialize(1000000 / audio_sample_rate);
    Timer3.attachInterrupt(timer_interrupt);
    Timer3.start();

    pr("sample rate: "); prln(audio_sample_rate);
  }
  }*/

//---------------------------------------------------------------
inline void set_audio_sample_rate(int rate) {
  //change sample rate only if big difference
  //if (debug_now) {
  //  pr("--- sample rate: "); prln(rate);
  //}

  if (abs(audio_sample_rate - rate) > audio_sample_rate_step) {
    audio_sample_rate = rate;

    //Stop sound
    //audio_sample_rate_shifter = audio_sample_rate_shifters[index];
    Timer3.stop();

    //Update
    sound_update_speedup_values();  //update sound values

    //Start sound
    Timer3.detachInterrupt();
    Timer3.initialize(1000000 / audio_sample_rate);
    Timer3.attachInterrupt(timer_interrupt);
    Timer3.start();

    if (debug_now) {
      pr("new sample rate: "); prln(audio_sample_rate);
    }
  }
}


//----------------------------------------------------------
//Apply sliders
//----------------------------------------------------------
extern const int diff_keep_max;
void sound_set_sliders(int slider_tone, int slider_sample_rate, int slider_diffusion) {
  tone_adjust_ = 3 * float(slider_tone - 512) / 512.0; //-3..3
  int rate = map(slider_sample_rate, 0, 1023, audio_sample_rate1, audio_sample_rate0);
  set_audio_sample_rate(rate);

  //diffusion
  set_diff_keep(map(slider_diffusion,0,1023,0,diff_keep_max));
}

//  alg_step10bit 0..1023 - kind of "level of sound generation", the higher - the more dithering
//  alg_diff10bit 0..1023 - memory of dithering, the higher - the more harshness
/*void sound_set_1bitparams(int alg_step10bit, int alg_diff10bit) {
  audio_volume = ((long int)alg_step10bit) * (max_audio_volume - min_audio_volume) / 1024 + min_audio_volume;
  diff_keep = ((long int)alg_diff10bit) * diff_keep_denom / 1024;

  //Formula for threshold
  thresh_sound = ((long int) audio_volume) * diff_keep / (diff_keep_denom) + 1; //I expect to use 2*diff_keep_denom, but beep occurs:)


  //Debug print - uncomment to print sliders values
  //pr("audio_volume "); pr(audio_volume);
  //pr(", diff_keep float "); pr(float(diff_keep)/diff_keep_denom);
  //pr(", thresh_sound "); pr(thresh_sound);
  //prln();
  }*/


//---------------------------------------------------------------
//Synthesis
//---------------------------------------------------------------
//frequencies of sounds
//0 means note off
long int freq1 = 0;
long int freq2 = 0;
long int freq3 = 0;
unsigned long long freq1_wave_n = 0;    //freq1 * wave_n for speedup computing
unsigned long long freq2_wave_n = 0;
unsigned long long freq3_wave_n = 0;

//---------------------------------------------------------------
//Set playing frequencies
//0 means note off
/*void set_freqs(int freq1_, int freq2_, int freq3_, int freq4_) {
  freq1 = freq1_;
  freq2 = freq2_;
  freq3 = freq3_;
  freq1_wave_n = freq1 * wave_n;
  freq2_wave_n = freq2 * wave_n;
  freq3_wave_n = freq3 * wave_n;
  }*/

//---------------------------------------------------------------
//Set playing notes
//-1 means note off, that os freqi=0
void sound_set_notes(char midi_note1, char midi_note2, char midi_note3, char midi_note4, char base_note) {
  
  freq1 = (midi_note1 != -1) ? m_to_f_int(midi_note1 + base_note) : 0;
  freq2 = (midi_note2 != -1) ? m_to_f_int(midi_note2 + base_note) : 0;
  freq3 = (midi_note3 != -1) ? m_to_f_int(midi_note3 + base_note) : 0;

  sound_update_speedup_values();

  if (debug) {
    pr("   freq "); pr(freq1);
    pr(" "); pr(freq2);
    pr(" "); pr(freq3);
    //pr(" "); pr(int(freq4));
    prln();
  }
}

//---------------------------------------------------------------
//update values for fast computing
//base formula:
//    wave_table[((phase * freq1_wave * wave_n) /  audio_sample_rate) % wave_n]
//optimize 1:
//    freq1_wave_n = freq1 * wave_n;
//    wave_table[((phase * freq1_wave_n) /  audio_sample_rate) % wave_n]
//optimize 2:
//    freq1_wave_n = freq1 * wave_n * 16777216 / audio_sample_rate; //2^24
//    wave_table[((phase * freq1_wave_n) >> 24) % wave_n]

#define SOUND_WAVE_SHIFT (24)

void sound_update_speedup_values() {
  double koef = double (wave_n) * 16777216 / audio_sample_rate;
  freq1_wave_n = (unsigned long long)(freq1 * koef);
  freq2_wave_n = (unsigned long long)(freq2 * koef);
  freq3_wave_n = (unsigned long long)(freq3 * koef);

  //debug print
  //pr("freq koef ");
  //pr(double(koef)); pr(" ");
  //pr(double(freq1_wave_n)); pr(" ");
  //pr(double(freq2_wave_n)); pr(" ");
  //prln(double(freq3_wave_n));
}

//---------------------------------------------------------------
void sound_setup() {
  pr("Audio output pin: "); prln(pin_audio_output);
  pr("Starting with audio sample rate: "); prln(audio_sample_rate);

  //Set port/pin mode for Arduino Mega for buzzer
  //Ports in Arduino https://www.arduino.cc/en/Reference/PortManipulation
  //Pins in Arduino Mega https://www.arduino.cc/en/Hacking/PinMapping2560
  //pin 2 is 4 bit of Port E (in scheme PE2)
  //DDRE = B00010000;    //Can' do it because all other PORTE pins will be disabled, but we need enable for sliders power
  pinMode(2,OUTPUT);

  //test_notes();
  //Start interrupt for sound generation
  //set_audio_sample_rate_index(1);  //Timer3.initialize(1000000 / audio_sample_rate);

  pr("Polyphony: "); prln(POLYPHONY);

  //set default tone and sample rate
  sliders_loop();
}


//----------------------------------------------------------
//Sound generation
//----------------------------------------------------------
//1-bit sound diffusion parameters

//Audio volume - changed by ADSR
const int audio_volume_min = 0;    
const int audio_volume_max = 127;
const int audio_volume_shift = 7; // << 7 instead "/audio_volume_max"

int audio_volume = 127; //volume of the sound, controlled by ADSR

//This value is used in LFO filter:
int audio_step = //127 * POLYPHONY; //step of diffusion subtraction for 1 bit algorithm - 1..127, kind of threshold for sound
  audio_volume_max * 90 / 127;  //more phatty than just max;

  
int diff_keep = 16; //64; //decaying diffusion 0..256, 0 - no diffusion, 256 - keep all diffusion
const int diff_keep_max = 256;
const int diff_keep_shift = 8;

//threshold for switching buzzer, must be so that not to allow "silence beep"
int thresh_sound = 1; //20;
//audio_volume / 2; //but for diff_keep <= 50 can be lower

//Formula, but not tested for this sound
//const int thresh_sound = ((long int) audio_volume) * diff_keep / (2*diff_keep_denom) + 1;

//Variable of sound output
long int sound_value = 0; //error diffusion value,   also affected by ADSR
long int sound_temp = 0;

//----------------------------------------------------------
//set decaying diffusion 0..256, 0 - no diffusion, 256 - keep all diffusion
void set_diff_keep(int v) {
  diff_keep = v;
}
//----------------------------------------------------------
//process timer interrupt event
//sound generator
long long phase = 0;

//We should make this function as fast as possible, and trying to omit "/" and "%" operations
void timer_interrupt() {
  //decay accumulated diffusion, because in opposite case zero values will give constant high-tone
  sound_value = (sound_value * diff_keep) >> diff_keep_shift;    

  //----------------------------------
  //if (mic_button) {
  //Microphone
  //  sound_value += (analogRead(A0) - 512) >> 2; //0..1023 -> -512..511 -> -128..127   - we do so because mic is quiet...
  //}
  //----------------------------------
  //else
  {
    //wave table
    //phase - is changed audio_sample_rate times per second
    //wave_n - length of wavetable
    //freq - desired frequency
    //ph - must go freq times 0..wave_n-1
    //optimize 2:
    //    freq1_wave_n = freq1 * wave_n * 1048576 / audio_sample_rate; //2^24
    //    wave_table[((phase * freq1_wave_n) >> 24) % wave_n]
    sound_temp = 0;
    if (freq1) sound_temp += wave_table[((phase * freq1_wave_n) >> SOUND_WAVE_SHIFT) % wave_n];
    if (freq2) sound_temp += wave_table[((phase * freq2_wave_n) >> SOUND_WAVE_SHIFT) % wave_n];
    if (freq3) sound_temp += wave_table[((phase * freq3_wave_n) >> SOUND_WAVE_SHIFT) % wave_n];

    sound_value += sound_temp;//(sound_temp * audio_volume) >> audio_volume_shift;
  }
  //----------------------------------

  phase++;

  //Output audio sample to buzzer using dithering algorithm threshold+decay
  //we use ports here, so also disable all PORTE pins at once!
  
  if (sound_value >= thresh_sound) {
    PORTE = B00010000; //buzzer ON   
    //PORTE |= B00010000; //better, because doesn't affect other pins, but very slow
    sound_value -= audio_step;       //diffusion propagation
  }
  else {
    if (sound_value <= -thresh_sound) {
      PORTE = B00000000; //buzzer OFF;
      //PORTE &= B11101111; //better, because doesn't affect other pins, but very slow
      sound_value += audio_step;       //diffusion propagation
    }
  }

}


//----------------------------------------------------------
unsigned long int sound_time = 0;

void sound_loop() {
  /*unsigned long time = millis();
  if (time > sound_time + 200) {
    sound_time = time;
    long int time = millis();
    //pr("time: ");
    //pr(time);
    //pr("\t");
    //pr("counter: ");
    //pr(t);

    //pr("\trate: ");
    //prln(double(t) / (time / 1000.0));
  }*/
}


//---------------------------------------------------------------
