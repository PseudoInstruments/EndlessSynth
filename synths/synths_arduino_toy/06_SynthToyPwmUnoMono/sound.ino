//---------------------------------------------------------------
//Sound generation
//---------------------------------------------------------------
//Using library TimerOne for working with Arduino's Timer1
#include <TimerOne.h>
//Note: if you get error message here, then install "TimerOne" library using Library Manager


const byte pin_audio = A0;  //we will use A0 as digital output pin!

const long int sample_rate = 8192;   //Sample rate of the audio generation
const int timer_period = long(1000000) / sample_rate;  //period for Timer1 working


const float pitch_band_range = 6;  //range of pitch bend of hemitones

byte sound_vol = 0;  //sound "volume", 0..255, changing by attack/release processes; precisely it's not volume, but PWM modulation
int sound_freq = 1; //current sound frequency for sound generation
byte sound_pwm = 0; //cuurent PWM value for sound generation

byte playing_note = 0; //currently playing note - it keeps here for "release" process even after user releases key

//PWM
const long int pwm_range = 256;
const int pwm_pow2 = 8;  //2^8 = pwm_range

long int pwm_ = 0;

//attack/release implemented as PWM change, we are using integer arithmetics instead floats to speed-up computing
const long int fade_range = (long int)1024*32; 
const int fade_pow2 = 15;  //2^... = fade_range
   
long int fade_pos_ = 0;    //position for attack/release, "_" marks it's changed in audio_loop(); value 0 - means min volume, value fade_range means max volume
long int fade_adder = 0;  //how much add to fade at each sound sample generation

//PWM phase
const long int phase_range = sample_rate*4;   //Nyqist and more
long int phase_ = 0;
long int phase_adder = 0;
long int momentary_pwm_ = 0;

unsigned long int time_ = 0;

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

  // Attack/release
  unsigned long int time = millis();
  unsigned long int dt = time - time_;
  time_ = time;

  fade_pos_ += fade_adder * dt;
  if (fade_adder > 0 && fade_pos_ > fade_range) fade_pos_ = fade_range;
  if (fade_adder < 0 && fade_pos_ < 0) fade_pos_ = 0;
  //For usage below: sound volume is fade_pos_/fade_range

  // PWM
  pwm_ = get_pwm();

  // Computing current sound frequency - made from key and pitch bend
  sound_freq = int(m_to_f_float(playing_note    //Note
                                + get_pitch_percents() * pitch_band_range * 0.01f)); //Pitch -100..100, so multiply by 0.01

  // Computing phase_adder:
  // "phase_" should walk "sound_freq" periods by one second, the period is "phase_range"
  // so "phase_" goes phase_range*sound_freq per second, or phase_range*sound_freq/sample_rate per audio step
  // - this is phase_adder
  phase_adder = float(phase_range) * sound_freq / sample_rate;

  // Debug print
  static byte print = 0;
  print = (print + 1) % 10;
  if (print == 0) {

    Serial.print("sound_freq: "); Serial.print(sound_freq);
    Serial.print("\tphase_adder: "); Serial.print(phase_adder);

    Serial.print("\tfade_adder: "); Serial.print(fade_adder);
    Serial.print("\tfade_pos_: "); Serial.print(fade_pos_);

    //Serial.print("\tpwm_: "); Serial.print(pwm_);
    //Serial.print("\tphase_range: "); Serial.print(phase_range);
    Serial.print("\tmomentary_pwm_: "); Serial.print(momentary_pwm_);  
    //Serial.print("\tphase"); Serial.print(phase_);
    Serial.println();

  }

}

//---------------------------------------------------------------
//start attack/release, dir==1 - attack, dir==-1 - release
void set_fade(int duration_ms, int dir) {
  //we work in milliseconds rate
  //need to go way fade_range at duration_ms milliseconds
  fade_adder = fade_range / duration_ms * dir;  //integer-arithmetics optimized from original formula: fade_range / (duration_ms * sample_rate / 1000);
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


void audio_out() {
  //Test of sound
  //static byte v = 0;
  //digitalWrite(A0,v);
  //v = 1-v;
  //return;

  // Sound phase
  phase_ += phase_adder;
  phase_ %= phase_range;
  // if phase_ < pwm*volume, then audio out = 1, else = 0;

  momentary_pwm_ = ((phase_range * pwm_ >> pwm_pow2) * fade_pos_) >> fade_pow2;  // / (pwm_range * fade_range);
  //if (phase_ < phase_range/2)   //For debugging - simple 50% PWM
  if (phase_ < momentary_pwm_) {
    digitalWrite(A0, 1);
  }
  else {
    digitalWrite(A0, 0);
  }
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
