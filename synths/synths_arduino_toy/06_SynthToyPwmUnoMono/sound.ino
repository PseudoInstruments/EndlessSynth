//---------------------------------------------------------------
//Sound engine
//---------------------------------------------------------------
//Using library TimerOne for working with Arduino's Timer1
#include <TimerOne.h>
//Note: if you get error message here, then install "TimerOne" library using Library Manager


const byte pin_audio = A0;  //we will use A0 as digital output pin!

const int sample_rate = 10000;   //Sample rate of the audio generation
//8192;  //2^13//power of 2 to faster divide

const int timer_period = long(1000000) / sample_rate;  //period for Timer1 working

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

}


//---------------------------------------------------------------
// audio_out() - Main sound function which is called by Timer 1 at rate SampleRate Hz

byte phase = 0; //it's fun trick: so phase range is 0..255 - we will use it for PWM computation

void audio_out() {
  /*if (ledState == LOW) {
    ledState = HIGH;
    blinkCount = blinkCount + 1;  // increase when LED turns on
    } else {
    ledState = LOW;
    }
    digitalWrite(led, ledState);*/

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
