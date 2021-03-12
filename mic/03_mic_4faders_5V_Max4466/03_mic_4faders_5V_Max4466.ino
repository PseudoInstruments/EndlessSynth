/* 03_mic_4faders_5V_Max4466
  Endless Mic Processor converts input sound to 1 bit using four faders, with 1V ADC ARef.
  New: 
  1) Using Mic module based on Max4466, it gives output centered at 2.5V and has gain regulator,
  so we use default ARef=5V, and no need in trimmer resistors!

  2) Using linear rotative faders 5kOhm. (May use 10kOhm).
  1 - volume - directly between pin 2 and audio output, without Arduino
  2 - sample rate, A3
  3 - diffusion, A4   (kind of release)
  4 - sensitivity, A5

  Program gets sound from A0 and outputs it to audio output(pin 2) with diffusion.
  Controller: Arduino Uno or Nano

  ----------------------------------------
  Adjustment before playing
  ----------------------------------------
  - Zero-value of mic: built-in LED lighing when zero-level is achieved, that is A0 is roughly equal to 512.

  ----------------------------------------
  Connection
  ----------------------------------------
  Audio input
  1) Mic module based on Max4466:
  connect it to Gnd, 5V, and output signal connect to A0.

  Audio output
  2) Audio output to mini-jack (or buzzer):
  Connect Gnd and pin 2 to fader 1 inputs,
  then connect mini-jack (buzzer) to the faders 1 output and Gnd.

  Faders 2,3,4
  3) Connect faders' 1,2,3 inputs to Gnd and 5V.
  Connect their outputs to A3,A4,A5.

  ----------------------------------------
  Programming details
  ----------------------------------------
  By default Arduino ADC works on 8Khz - ok for us.
  But can speedup:
  https://www.instructables.com/Make-Your-Own-Spy-Bug-Arduino-Voice-Recorder/
  Recorder:
  https://github.com/TMRh20/TMRpcm
  http://microsin.net/programming/avr/real-time-digital-audio-processing-using-arduino.html

*/

const byte pin_buz = 2; //Audio output pin
const byte pin_led = 13;  //Built-in led pin

//fader1 affects volume output directly, without arduino
const byte sample_rate_apin = A3;   //sample rate
const byte diffusion_rate_apin = A4;   //diffusion
const byte sensitivity_rate_apin = A5;   //sensitivity

const unsigned int analog_min = 0;     //Note: adjust this values is faders gives not full range
const unsigned int analog_max = 1023;

int debug = 0;    //control from keyboard to begin debugging

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("Endless Mic Processor with 4 faders, 5V, Max4466, v. 1.3 for Arduino Uno, Nano");
  Serial.println("Program gets sound from A0 and outputs to pin 2 using diffusion (kind of release).");
  Serial.println("4 faders: volume - between pin2 and audio output; sample rate - A3, diffusion - A4, sensitivity - a5.");

  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  pinMode(pin_buz, OUTPUT); //activate audio output

  pinMode(pin_led, OUTPUT); //activate led


  //ARef - set to 1.1V, for increasing sensitivity 
  //analogReference(INTERNAL); - commented here because using Mic module Max4466, which outputs center at 2.5V 
}


//--------------------------------------------------------------
//Utils
int clampi(int i, int a, int b) {
  if (a > b) {
    int t = a; a = b; b = t;
  }
  if (i < a) return a;
  if (i > b) return b;
  return i;
}

int mapi_clamp(int i, int a, int b, int A, int B) {
  return clampi(map(i, a, b, A, B), A, B);
}


//--------------------------------------------------------------
//Constants
const int audio_delay_mcs0 = 500;
const int audio_delay_mcs1 = 0;
const int audio_thresh_fader0 = 512;
const int audio_thresh_fader1 = 900;

const int audio_thresh_hyster = 2;

//Sound parameters
int audio_delay_mcs = 10;   //delay in sound loop

//zero value range for adjusting trimming resistor - inside range the led is lighting
const int audio_thresh_adj0 = 512 - 5;
const int audio_thresh_adj1 = 512 + 5;


int audio_thresh0 = 512;     //two thresholds for hysteresis (stability)
int audio_thresh1 = audio_thresh0 + audio_thresh_hyster;

int audio_diff_step = 400; //90;  //127

const int audio_diffusion0 = 0;
const int audio_diffusion1 = 256;
int audio_diff_keep = 16; //64; //decaying diffusion 0..256, 0 - no diffusion, 256 - keep all diffusion
const int diff_keep_denom = 256;

int thresh_sound = 20;

int audio_input_ = 0;
int sound_value = 0;


int loops_ = 1000;

//--------------------------------------------------------------
inline void control_step() {
  if (Serial.available() > 0) { //we expect only "1" rare to on/off debugging
    int key = Serial.read();
    if (key == '1') {
      debug = 1 - debug;
      Serial.print("Debug "); Serial.println(debug);
    }
  }

  //TODO make each step at separate loop to minimize delay
  int fader2 = analogRead(sample_rate_apin);  //0..1023
  int fader3 = analogRead(diffusion_rate_apin);  //0..1023
  int fader4 = analogRead(sensitivity_rate_apin);  //0..1023

  //Sample rate
  audio_delay_mcs = mapi_clamp(fader2, analog_min, analog_max, audio_delay_mcs0, audio_delay_mcs1);

  //Diffusion
  audio_diff_keep = mapi_clamp(fader3, analog_min, analog_max, audio_diffusion0, audio_diffusion1);

  //Sensitivity
  audio_thresh0 = mapi_clamp(fader4, analog_min, analog_max, audio_thresh_fader1, audio_thresh_fader0); //reverted range
  audio_thresh1 = audio_thresh0 + audio_thresh_hyster;

  //lighting LED if audio inside adjusting range - helping for adjusting trimming resistor
  if (audio_input_ >= audio_thresh_adj0 && audio_input_ <= audio_thresh_adj1) {
    digitalWrite(pin_led, HIGH);
  }
  else {
    digitalWrite(pin_led, LOW);
  }

  //debug print
  if (debug) {
    //use audio input for setting up trimmer resistor so it print 512
    Serial.print("audio input "); Serial.print(audio_input_);
    Serial.print("  audio loops "); Serial.print(loops_);
    Serial.print("  faders "); Serial.print(fader2);
    Serial.print(","); Serial.print(fader3);
    Serial.print(","); Serial.print(fader4);
    Serial.print("  delay_mcs "); Serial.print(audio_delay_mcs);
    Serial.print("  diffusion "); Serial.print(audio_diff_keep);
    Serial.print("  thresh "); Serial.print(audio_thresh0);

    Serial.println();
  }
}



//--------------------------------------------------------------
void loop() {

  //run control step
  control_step();

  //run audio loop
  loops_ = map(audio_delay_mcs, audio_delay_mcs0, audio_delay_mcs1, 500, 40); //1000, 10);
  //NOTE - this simple formula gives good sound but in the middle part of fader 2 it freezes a control rate a bit

  //this algorithm more precise, but sounding not so good:

  //long int freq = (1000000 / (audio_delay_mcs + 100));
  //Serial.println(freq);
  //loops_ = map(freq, 9000, 200, 1000, 30); //TODO make as params and tune. I see debug speed to tune
  //loops_ = constrain(loops_, 30, 1000);

  unsigned long time0 = micros();
  for (int i = 0; i < loops_; i++) {
    //decay accumulated diffusion, because in opposite case zero values will give constant high-tone
    sound_value = (sound_value * audio_diff_keep) >> 8;    // >> 8 means / diff_keep_denom //sound_value >> 2;

    //get sample
    audio_input_ = analogRead(A0);

    //output
    //two thresholds for hysteresis (stability)
    if (audio_input_ >= audio_thresh1) sound_value += (audio_input_ - audio_thresh1);
    else {
      if (audio_input_ <= audio_thresh0) sound_value += (audio_input_ - audio_thresh0);
    }

    //audio output
    if (sound_value >= thresh_sound) {
      digitalWrite(pin_buz, HIGH);    //buzzer ON
      sound_value -= audio_diff_step;       //diffusion propagation
    }
    else {
      if (sound_value <= -thresh_sound) {
        digitalWrite(pin_buz, LOW);    //buzzer OFF
        sound_value += audio_diff_step;       //diffusion propagation
      }
    }

    //delay
    delayMicroseconds(audio_delay_mcs);
  }

  //estimate sample rate, runs once
  static byte was_measured = 0;
  if (!was_measured) {
    was_measured = 1;
    unsigned long delta = micros() - time0;
    long int audio_sample_rate = 1000000.0 * loops_ / delta;
    Serial.print("Computed audio sample rate: "); Serial.println(audio_sample_rate);
  }

}
//--------------------------------------------------------------
