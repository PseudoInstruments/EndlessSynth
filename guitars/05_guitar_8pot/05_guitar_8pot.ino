/* Endless Guitar 05_guitar_8pot.

  This guitar features 1 volume and 7 control pots.
  
  Controller: Nano

  -------------------------------------------
  I use 4th bass string in guitar.
  
  -------------------------------------------
  Potentiometers:
  1 - controls volume - connected directly to DAC and Arduino audio output.
  2 - 
  3 - 
  4 - 
  5 -
  6 - 
  7 - 
  8 -

  ----------------------------------------
  Connection
  ----------------------------------------
  Audio input
  1) Guitar pickup:
  connect it to Gnd, 5V, and output signal connect to A0.

  Audio output
  2) Audio output to mini-jack (or buzzer). If you have no pot for volume control,
  connect audio output directly to pin D2 (signal) and D4 (Gnd).
  If you have slider for volume control (I use potentiometer 10 kOhm), connect Gnd and pin 2 to slider's inputs,
  then connect mini-jack (buzzer) slider's output and Gnd.

  4) Sliders - in this sketch are used two sliders (I mean potentiometers 10kOhm) for controlling sound parameters.
  A1
  A2
  A3
  A4
  A5
  A6
  A7


  So after start sketch, send to Monitor Port "1" to start debug print.

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

const byte pin_audio_out = 2; //Audio output
const byte pin_audio_out_gnd = 4; //Audio output Gnd


const byte pin_led = 13;  //Built-in led pin

const byte N = 7;   //number of control pots
int Pot[N] = {0,0,0,0,0,0,0};
int &Pot_SRate = Pot[0];
int &Pot_Sens = Pot[1];


const unsigned int analog_min = 20;     //it's appears minimal value is 20, not 0 in the current setup
const unsigned int analog_max = 1023;


int debug = 0;    //control from keyboard to begin debugging

//--------------------------------------------------------------
void pinModePower(byte pin, byte value) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, value);
}

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("EndlessSynth Guitar, version 05_guitar_8pot_aref for Arduino Nano");
  Serial.println("Guitar pickup: A0, Pots: A1,...,A7, Audio output: D2");
  Serial.println("Volume Pot: signal D2 and audio output, D4 - audio Gnd.");
  Serial.println("Send '1' to on/off debug print");

  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  pinModePower(pin_audio_out, LOW); //activate audio output
  pinModePower(pin_audio_out_gnd, LOW); //activate audio output Gnd

  pinModePower(pin_led, LOW); //activate led

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
  //TODO separate on steps
  Pot[0] = analogRead(A1);
  Pot[1] = analogRead(A2);
  Pot[2] = analogRead(A3);
  Pot[3] = analogRead(A4);
  Pot[4] = analogRead(A5);
  Pot[5] = analogRead(A6);
  Pot[6] = analogRead(A7);

  //Set pots to sound params
  audio_delay_mcs = mapi_clamp(Pot_SRate, analog_min, analog_max, audio_delay_mcs0, audio_delay_mcs1);
  audio_thresh = mapi_clamp(Pot_Sens, analog_min, analog_max, audio_thresh_slider1, audio_thresh_slider0); //reverted range

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
    Serial.print("  Pots: "); 
    for (int i=0; i<N; i++) {
      Serial.print(Pot[i]); Serial.print(" ");
    }
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
      digitalWrite(pin_audio_out, HIGH);
    }
    else {
      digitalWrite(pin_audio_out, LOW);
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
