//---------------------------------------------------------------
//Sound generation
//---------------------------------------------------------------
//Using library TimerOne for working with Arduino's Timer1
#include <TimerOne.h>
//Note: if you get error message here, then install "TimerOne" library using Library Manager


const byte pin_audio = A0;  //we will use A0 as digital output pin!

const long int sample_rate = 8192;   //Sample rate of the audio generation
const byte sample_rate_pow2 = 13; //2^13 = 8192//power of 2 to faster divide
const int timer_period = long(1000000) / sample_rate;  //period for Timer1 working


const float pitch_band_range = 6;  //range of pitch bend of hemitones

byte sound_vol = 0;  //sound "volume", 0..255, changing by attack/release processes; precisely it's not volume, but PWM modulation
int sound_freq = 1; //current sound frequency for sound generation
byte sound_pwm = 0; //cuurent PWM value for sound generation

byte playing_note = 0; //currently playing note - it keeps here for "release" process even after user releases key

//attack/release implemented as PWM change, we are using integer arithmetics instead floats to speed-up computing
long int fade_range = 500000;    //just a "big number"
long int fade_pos_ = 0;    //position for attack/release, "_" marks it's changed in audio_loop(); value 0 - means min volume, value fade_range means max volume
long int fade_adder = 0;  //how much add to fade at each sound sample generation


//sound phase 0..255: (((long long)phase * freq1 * 256) >> sample_rate_pow2) % 256];

//---------------------------------------------------------------
// Setup sound
void sound_setup() {
  //Audio pin
  pinMode(pin_audio, OUTPUT);
  digitalWrite(pin_audio, 0);

  // Starting timer for sound generation
  Timer1.initialize(timer_period);   //set timer's period
  Timer1.attachInterrupt(audio_out); //set function which is called by timer
}

//---------------------------------------------------------------
// Sound update at control rate
//This is not audio generator, but control-rate function for changing sound
void sound_update() {
  //Test of sound
  //digitalWrite(A0,1);
  //delay(2);
  //digitalWrite(A0,0);
  //delay(2);
  //return;
  //-------------

  //current frequency - made from key and pitch bend
  sound_freq = int(m_to_f_float(playing_note    //Note
                                + get_pitch_percents() * pitch_band_range * 0.01f)); //Pitch -100..100, so multiply by 0.01

  // Debug print
  /* 
  Serial.print("sound_freq: "); Serial.print(sound_freq);
  Serial.print("\tfade_adder: "); Serial.print(fade_adder);
  Serial.print("\tfade_pos_: "); Serial.print(fade_pos_);
  Serial.println();
 */

}

//---------------------------------------------------------------
//start attack/release, dir==1 - attack, dir==-1 - release
void set_fade(int duration_ms, int dir) {
  //need to go way fade_range at duration_ms/1000 seconds, that is duration_ms/1000*sample_rate samples
  fade_adder = fade_range * 1000 / (duration_ms * sample_rate) * dir;  //integer-arithmetics optimized from original formula: fade_range / (duration_ms * sample_rate / 1000);
}

//---------------------------------------------------------------
//Key events
//---------------------------------------------------------------
//Note that we change attack_on, release_on in the end of functions becaue it's used IN "anothe thread" - in audio_loop()
void key_pressed(byte note) { //key newly pressed - start sounding with attack
  Serial.print("note pressed "); Serial.println(note);
  playing_note = note;
  //start attack
  int attack_ms = get_attack_ms();
  if (attack_ms == 0) {
    fade_pos_ = fade_range;
    fade_adder = 0;
  }
  else {
    set_fade(attack_ms, 1);
    fade_pos_ = 0;  //retrig fade position
  }
}

//---------------------------------------------------------------
void key_changed(byte note) { //key changed - just change tone, without attack
  Serial.print("note changed "); Serial.println(note);
  playing_note = note;
}

//---------------------------------------------------------------
void key_released() {  //key released - start sound to fade
  Serial.println("key released ");
  //start release
  int release_ms = get_release_ms();
  if (release_ms == 0) {
    fade_pos_ = 0;
    fade_adder = 0;
  }
  else {
    set_fade(release_ms, -1);
    //note, we don't do here "fade_pos_ = fade_range" so sound will dissapear smoothly even if attack is not finished
  }
}

//---------------------------------------------------------------
// audio_out() - Main sound function which is called by Timer 1 at rate SampleRate Hz

byte phase = 0; //it's fun trick: so phase range is 0..255 - we will use it for PWM computation

void audio_out() {
  //Test of sound
  //static byte v = 0;
  //digitalWrite(A0,v);
  //v = 1-v;
  //return;

  // Attack/release
  fade_pos_ += fade_adder;
  if (fade_adder > 0 && fade_pos_ > fade_range) fade_pos_ = fade_range;
  if (fade_adder < 0 && fade_pos_ < 0) fade_pos_ = 0;
  //For usage below: sound volume is fade_pos_/fade_range


  //frequencies of sounds
  //0 means note off
  //long int freq1 = 0;
  //  freq1 = (midi_note1 != -1) ? m_to_f_int(midi_note1 + base_note) : 0;



  //We should make this function as fast as possible, and trying to omit "/" and "%" operations
  //phase - is changed audio_sample_rate times per second
  //wave_n - length of wavetable
  //freq - desired frequency
  //ph - must go freq times 0..wave_n-1
  //  if (freq1) sound_value += wave_table[(((long long)phase * freq1 * wave_n) >> shift_audio) % wave_n];
  //phase++;
}

//---------------------------------------------------------------
//Utility functions for getting note frequency
//http://newt.phys.unsw.edu.au/jw/notes.html

//Convert midi note to frequency
int m_to_f_int(int midi_note) {
  return m_to_f_float(midi_note);
}

//float version
float m_to_f_float(float midi_note) {
  return pow(2, (midi_note - 69) / 12) * 440; //Hz
  //note: for midi_note>100 sometimes occurs overflow
}

//---------------------------------------------------------------
