/* Program gets sound from microphone (A0)
  and immediately outputs it to buzzer (pin 2) using threshold+decay diffusion algorithm,
  see "sound_tests/12_PolyphonyMega_diffusion_thresholding" for the details on this
  Can be used for electo-guitar connection too.

  This is simpler than synths because don't uses timer interrupts.

  Controller: Arduino Uno or Mega

  ----------------------------------------
  Connection
  ----------------------------------------
  1) Microphone:
  Take microphone unit for Arduino:
  connect it to Gnd, 5V, and output signal connect to A0.

  2) Trimmer resistor for pulling-up microphone:
  Note, that microphone unit gives -2.5V...2.5V output,
  so to digitize signal carefully we need to pull up it to 0..5V range for A0.
  As a solution, I use compact 10KOm trimmer resistor,
  connect left and right pins to Gnd, 5V, and output to A0 too.
  Next, I adjust trimmer resistor to obtain 2.5V when Mic is in a silence (or, equally, 512 on A0).
  Now microphone outputs 0..5V to A0!

  3) Buzzer (or audio output) to pin 2.

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

const byte pin_buz = 2; //Buzzer pin

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Sound passtrough test (threshold+decay diffusion), v. 1.1 for Arduino Uno or Mega");
  Serial.println("Program gets sound from microphone (A0) and immediately outputs");
  Serial.println("it to buzzer (pin 2) using threshold+decay diffusion algorithm.");
  Serial.println("Note: please use trimmer resistor 10KOhm to move silence mic level to 2.5V (or, equally, 512 on A0)");

  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  pinMode(pin_buz, OUTPUT); //activate buzzer

}

//--------------------------------------------------------------
//1-bit sound diffusion

long int sound_value = 0;
const int diff_step = //kind of threshold for sound
  50; //70; //127;    //just 127 - to make polyphony sounding more "phatty"

const int diff_keep = 32; //50;  //decaying diffusion in 0..128, 0 - no diffusion, 1 - keep all diffusion
const int diff_keep_denom = 128; //denominator for diff_keep;

//threshold for switching buzzer, must be so that not to allow "silence beep"
//Note: formula may be not so good, need adjustment between sensitive but beep and no beep but no sensitive.
const int thresh_sound = ((long int) diff_step + 1) * diff_keep / (diff_keep_denom) + 1; 

//--------------------------------------------------------------
void loop() {

  //run loop to measure time and deduce sample rate
  int n = 1000;
  int v;
  int vmin = 1023;
  int vmax = 0;
  unsigned long time0 = micros();
  for (int i = 0; i < n; i++) {
    //get sample
    sound_value = sound_value * diff_keep / 128;  //decaying diffusion
    v = analogRead(A0);
    
    sound_value += (v >> 2) - 128; //0..1023 -> 0..255 -> -128..127
    //output
    if (sound_value >= thresh_sound) {
      digitalWrite(pin_buz, HIGH); //buzzer ON
      sound_value -= diff_step;       //diffusion propagation
    }
    else {
      if (sound_value <= -thresh_sound) {
        digitalWrite(pin_buz, LOW); //buzzer OFF;
        sound_value += diff_step;       //diffusion propagation
      }
    }

    //compute range
    vmin = min(vmin, v);
    vmax = max(vmax, v);

    //delay
    //delayMicroseconds(10);  
    //Need to adjust delay to achieve 8000Hz rate - this is default rate of Arduino ADC
    //Though, it can be increased.
     
  }

  //estimate sample rate, runs once
  static byte was_measured = 0;
  if (!was_measured) {
    was_measured = 1;
    unsigned long delta = micros() - time0;
    long int audio_sample_rate = 1000000.0 * n / delta;
    Serial.print("Computed audio sample rate: "); Serial.println(audio_sample_rate);
  }

  //debug print of the range - use for setting up trimmer resistor so it print 512
  Serial.print(vmin); Serial.print(" - "); Serial.println(vmax);

}
