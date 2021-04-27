/* Endless Guitar is a 1-string 1-bit guitar with controllable volume, sample rate and sensitivity.

  Program gets sound from A0 and immediately outputs it to audio output (pin 2) using thresholding, without diffusion.
  Controller: Arduino Uno or Nano

  This sketch and electric scheme is a simplification comparing previous one: 
  here we not using trimmer resistors, and just crop negative input voltage values.

  -------------------------------------------
  I use 2th bass string in guitar.
  
  -------------------------------------------
  Potentiometers:
  1 - controls volume - connected directly to DAC and Arduino audio output.
  2 - controls sample rate.
  3 - controls sensitivity.

  ----------------------------------------
  Connection
  ----------------------------------------
  Audio input
  1) Guitar pickup:
  connect it to Gnd, 5V, and output signal connect to A0.

  Audio output
  2) Audio output to mini-jack (or buzzer). If you have no pot for volume control,
  connect audio output directly to pin 2 and Gnd.
  If you have slider for volume control (I use potentiometer 10 kOhm), connect Gnd and pin 2 to slider's inputs,
  then connect mini-jack (buzzer) slider's output and Gnd.

  4) Sliders - in this sketch are used two sliders (I mean potentiometers 10kOhm) for controlling sound parameters.
  From Arduino we will use output power: pin 5 (Gnd) and pin 6 (5V).

  The final goal is to connect:
  A4 - sample rate
  A5 - sensitivity

  Connect both sliders inputs to pin 5 (Gnd) and pin 6 (5V).
  Finally, connect sliders outputs to A4 and A5.

  So after start sketch, send to Monitor Port "1" to start debug print,
  and check that signal from guitar pickup and sliders 2 and 3 goes properly.

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

const byte pin_buz = 2; //Audio output
const byte pin_led = 13;  //Built-in led pin

const byte sliders_gnd_pin = 5;
const byte sliders_5v_pin = 6;

//slider1 affects volume output directly, without arduino
const byte slider2_analog_pin = A4;   //sample rate
const byte slider3_analog_pin = A5;   //pwm

const unsigned int analog_min = 20;     //it's appears minimal value is 20, not 0 in the current setup
const unsigned int analog_max = 1023;


int debug = 0;    //control from keyboard to begin debugging

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Guitar, version 04_guitar_sliders_simple for Arduino Uno, Nano");
  Serial.println("Guitar pickup: A0, Audio output: D2");
  Serial.println("Slider 1: between D2 and audio output, Slider 2: A4, Slider 3: A5.");
  Serial.println("Send '1' to on/off debug print");

  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  pinMode(pin_buz, OUTPUT); //activate buzzer

  pinMode(pin_led, OUTPUT); //activate led


  //sliders
  pinMode(sliders_gnd_pin, OUTPUT);
  pinMode(sliders_5v_pin, OUTPUT);
  digitalWrite(sliders_gnd_pin, LOW);
  digitalWrite(sliders_5v_pin, HIGH);

}


//--------------------------------------------------------------
//Constants
const int audio_delay_mcs0 = 0;
const int audio_delay_mcs1 = 1000;//400;  //audio loop delay range (sample rate control)
const int audio_thresh_slider0 = 0;
const int audio_thresh_slider1 = 20;   //sensitivity range

//Sound parameters
int audio_delay_mcs = 10;   //delay in sound loop

int audio_thresh = 5; //threshold for sound processing

int audio_input_ = 0;

int loops_ = 1000;


//--------------------------------------------------------------
//Utils

int clampi(int i, int a, int b) {
  if (a > b) { int t = a; a = b; b = t; }
  if (i < a) return a;
  if (i > b) return b;
  return i;
}

int mapi_clamp(int i, int a, int b, int A, int B) {
  return clampi(map(i, a, b, A, B), A, B);
}

//--------------------------------------------------------------
inline void control_step() {
  if (Serial.available() > 0) { //we expect only "1" rare to on/off debugging
    int key = Serial.read();
    if (key == '1') {
      debug = 1 - debug;
      Serial.print("Debug "); Serial.println(debug);
    }
  }

  //Read pots
  int slider2 = analogRead(slider2_analog_pin);  //0..1023
  int slider3 = analogRead(slider3_analog_pin);  //0..1023

  //Set pots to sound params
  audio_delay_mcs = mapi_clamp(slider2, analog_min, analog_max, audio_delay_mcs0, audio_delay_mcs1);
  audio_thresh = mapi_clamp(slider3, analog_min, analog_max, audio_thresh_slider1, audio_thresh_slider0); //reverted range

  //lighting LED if audio input works - to check guitar pickup
  if (audio_input_ > audio_thresh) {
    digitalWrite(pin_led, HIGH);
  }
  else {
    digitalWrite(pin_led, LOW);
  }

  //debug print
  if (debug) {
    //use audio input for setting up trimmer resistor so it print 512
    Serial.print("Audio input: "); Serial.print(audio_input_);
    Serial.print("  Pots: "); Serial.print(slider2);
    Serial.print(","); Serial.print(slider3);
    Serial.print("  audio_delay_mcs: "); Serial.print(audio_delay_mcs);
    Serial.print("  audio_thresh: "); Serial.print(audio_thresh);
    Serial.print("  audio loops: "); Serial.print(loops_);

    Serial.println();
  }
}


//--------------------------------------------------------------
void loop() {

  //run control step
  control_step();

  //run audio loop
  loops_ = map(audio_delay_mcs, audio_delay_mcs0, audio_delay_mcs1, 500, 40);

  unsigned long time0 = micros();
  for (int i = 0; i < loops_; i++) {
    //audio input
    audio_input_ = analogRead(A0);
    //audio output
    if (audio_input_ > audio_thresh) {
      digitalWrite(pin_buz, HIGH);
    }
    else {
      digitalWrite(pin_buz, LOW);
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
