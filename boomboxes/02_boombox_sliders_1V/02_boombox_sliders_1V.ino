/* Simple boombox code with improved sensitivity 1.1V and sliders
  .

  Program gets sound from microphone (A0)
  and immediately outputs it to buzzer (pin 2) using simple thresholding, without diffusion.
  Can be used for electo-guitar connection too.


  Controller: Arduino Uno

  ----------------------------------------
  Connection
  ----------------------------------------
  1) Microphone:
  Take microphone unit for Arduino:
  connect it to Gnd, 5V, and output signal connect to A0.

  2) Trimmer resistor for pulling-up microphone:
  Note, that microphone unit gives -0.5V...0.5V output,
  so to digitize signal carefully we need to pull up it to 0..5V range for A0.
  As a solution, I use compact 10KOm trimmer resistor,
  connect left and right pins to Gnd, 5V, and output to A0 too.
  Next, I adjust trimmer resistor to obtain 2.5V when Mic is in a silence (or, equally, 512 on A0).
  Now microphone outputs 0..5V to A0!

  3) Buzzer or audio output passed through slider 1 to pin 2.

  4) Three sliders:
  slider 1 - volume, passed from pin 2 to slider to audio output
  slider 2 - sample rate to A4
  slider 3 - sensitivity to A5

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

//slider1 affects volume output directly, without arduino
const byte slider2_analog_pin = A4;   //sample rate
const byte slider3_analog_pin = A5;   //pwm

int debug = 0;    //control from keyboard to begin debugging

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("Endless Boombox with sliders improved sensitivity, v. 1.2 for Arduino Uno or Mega");
  Serial.println("Program gets sound from microphone (A0) and immediately outputs");
  Serial.println("it to buzzer (pin 2) using thresholding.");
  Serial.println("slider 1 - between pin2 and audio output, slider 2 - A4, slider 3 - A5.");
  Serial.println("Note: please adjust trimmer resistor 10KOhm to move silence mic level 512 on A0");

  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  pinMode(pin_buz, OUTPUT); //activate buzzer

  //ARef - set to 1.1V, for increasing sensitivity
  analogReference(INTERNAL);
}

//--------------------------------------------------------------
//Constants
const int audio_delay_mcs0 = 0;
const int audio_delay_mcs1 = 1000;//400;
const int audio_thresh_slider0 = 512;
const int audio_thresh_slider1 = 512 + 40;
const int audio_thresh_hyster = 2;

//Sound parameters
int audio_delay_mcs = 10;   //delay in sound loop

int audio_thresh0 = 512;     //two thresholds for hysteresis (stability)
int audio_thresh1 = audio_thresh0 + audio_thresh_hyster;

int audio_input_ = 0;

int loops_ = 1000;

inline void control_step() {
  if (Serial.available() > 0) { //we expect only "1" rare to on/off debugging
    int key = Serial.read();
    if (key == '1') {
      debug = 1 - debug;
      Serial.print("Debug "); Serial.println(debug);
    }
  }

  //TODO make each step at separate loop to minimize delay
  int slider2 = analogRead(slider2_analog_pin);  //0..1023
  int slider3 = analogRead(slider3_analog_pin);  //0..1023

  audio_delay_mcs = map(slider2, 0, 1023, audio_delay_mcs0, audio_delay_mcs1);
  audio_thresh0 = map(slider3, 0, 1023, audio_thresh_slider1, audio_thresh_slider0); //reverted range
  audio_thresh1 = audio_thresh0 + audio_thresh_hyster;

  //debug print
  if (debug) {
    //use audio input for setting up trimmer resistor so it print 512
    Serial.print("audio input "); Serial.print(audio_input_);
    Serial.print("  audio loops "); Serial.print(loops_);
    Serial.print("  sliders "); Serial.print(slider2);
    Serial.print(","); Serial.print(slider3);
    Serial.print("  audio_delay_mcs "); Serial.print(audio_delay_mcs);
    Serial.print("  audio_thresh "); Serial.print(audio_thresh0);

    Serial.println();
  }
}
//--------------------------------------------------------------
void loop() {

  //run control step
  control_step();

  //run audio loop
  loops_ = map(audio_delay_mcs, audio_delay_mcs0, audio_delay_mcs1, 500, 40); //1000, 10);
  //NOTE - this simple formula gives good sound but in the middle part of slider 2 it freezes a control rate a bit

  //this algorithm more precise, but sounding not so good:

  //long int freq = (1000000 / (audio_delay_mcs + 100));
  //Serial.println(freq);
  //loops_ = map(freq, 9000, 200, 1000, 30); //TODO make as params and tune. I see debug speed to tune
  //loops_ = constrain(loops_, 30, 1000);

  unsigned long time0 = micros();
  for (int i = 0; i < loops_; i++) {
    //get sample
    audio_input_ = analogRead(A0);
    //output
    //two thresholds for hysteresis (stability)
    if (audio_input_ >= audio_thresh1) digitalWrite(pin_buz, HIGH);
    else {
      if (audio_input_ <= audio_thresh0) digitalWrite(pin_buz, LOW);
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
