/* Endless Drums Pedal generates drum sound by pressing pedal - physical Sustain pedal.

  Controller: Arduino Uno or Nano
  ----------------------------------------
  Connection
  ----------------------------------------
  1) Audio output: Gns and D2
  
  2) Pedal Sustain - Gnd and D11 

  3) Sliders - this sketch uses two sliders (I mean potentiometers 10kOhm) for controlling sound parameters.
  A4 - sample rate
  A5 - diffusion
  And for pots connect Gnd, 5V
    
  //Note: nonlinearity - slider slows start TODO linearize sliders values


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

const byte pedal_pin = 11;  //Pedal pin

//slider1 affects volume output directly, without arduino
const byte slider2_analog_pin = A4;   //sample rate
const byte slider3_analog_pin = A5;   //pwm

const unsigned int analog_min = 20;     //it's appears minimal value is 20, not 0 in the current setup
const unsigned int analog_max = 1023;

int debug = 0;    //control from keyboard to begin debugging

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("Endless Drum Pedal, v. 1.1 for Arduino Uno, Nano");
  Serial.println("Audio output: pin D2, pedal: D11");
  Serial.println("slider 1 - between pin2 and audio output, slider 2 - A4, slider 3 - A5");
  Serial.println("Send '1' to enable debug print to console");
  
  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  pinMode(pin_buz, OUTPUT); //activate audio output

  pinMode(pin_led, OUTPUT); //activate led

  pinMode(pedal_pin, INPUT_PULLUP); //read pedal

  init_wave();
  
  //ARef - set to 1.1V, for increasing sensitivity
  //analogReference(INTERNAL);
}


//--------------------------------------------------------------
//Utils
int clampi(int i, int a, int b) {
  if (i < a) return a;
  if (i > b) return b;
  return i;
}

int mapi_clamp(int i, int a, int b, int A, int B) {
  return clampi(map(i, a, b, A, B), A, B);
}


//--------------------------------------------------------------

//Constants
const int audio_delay_mcs0 = 0;
const int audio_delay_mcs1 = 500;//400;
const int audio_thresh_slider0 = 512;
const int audio_thresh_slider1 = 512 + 40;
const int audio_thresh_hyster = 2;

//Sound parameters
int audio_delay_mcs = 10;   //delay in sound loop

//zero value range for adjusting trimming resistor - inside range the led is lighting
const int audio_thresh_adj0 = 512 - 5;
const int audio_thresh_adj1 = 512 + 5;


int audio_thresh0 = 512;     //two thresholds for hysteresis (stability)
int audio_thresh1 = audio_thresh0 + audio_thresh_hyster;

int audio_diff_step = 90;  //127

const int audio_diffusion0 = 0;
const int audio_diffusion1 = 256;
int audio_diff_keep = 16; //64; //decaying diffusion 0..256, 0 - no diffusion, 256 - keep all diffusion
const int diff_keep_denom = 256;

int thresh_sound = 20;

int audio_input_ = 0;
int sound_value = 0;


int loops_ = 1000;

//--------------------------------------------------------------
//Pedal
byte pedal_ = 0;

//Drum sample
const int wave_n = 512;
byte wave[wave_n];   //TODO store in bits

//sample pos
int pos_ = 0;

//--------------------------------------------------------------
void init_wave() {
  for (int i=0; i<wave_n; i++) {
    wave[i] = (random(100)<50)?0:1;
    //Serial.print(wave[i]);
    //Serial.print(" ");
  }
  //set last to zero to end sample with it
  wave[wave_n-1] = 0;
  
  
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

  //Reading pedal is inside sound loop
  //pedal_ = digitalRead(pedal_pin)?0:1;

  //Led on pedal
  if (pedal_) digitalWrite(pin_led, HIGH);
  else digitalWrite(pin_led, LOW);

  //TODO make each step at separate loop to minimize delay
  int slider2 = analogRead(slider2_analog_pin);  //0..1023
  int slider3 = analogRead(slider3_analog_pin);  //0..1023

  audio_delay_mcs = mapi_clamp(slider2, analog_min, analog_max, audio_delay_mcs0, audio_delay_mcs1);
  //audio_thresh0 = mapi_clamp(slider3, analog_min, analog_max, audio_thresh_slider1, audio_thresh_slider0); //reverted range
  //audio_thresh1 = audio_thresh0 + audio_thresh_hyster;

  audio_diff_keep = mapi_clamp(slider3, analog_min, analog_max, audio_diffusion0, audio_diffusion1);

 

  //debug print
  if (debug) {
    //use audio input for setting up trimmer resistor so it print 512
    Serial.print("pedal "); Serial.print(pedal_);
    Serial.print("  audio loops "); Serial.print(loops_);
    Serial.print("  sliders "); Serial.print(slider2);
    Serial.print(","); Serial.print(slider3);
    Serial.print("  audio_delay_mcs "); Serial.print(audio_delay_mcs);
    Serial.print("  audio_thresh "); Serial.print(audio_thresh0);
    Serial.print("  audio_diff "); Serial.print(audio_diff_keep);

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
  static byte last_pedal = 0;
  
  for (int i = 0; i < loops_; i++) {
    //pedal 
    pedal_ = digitalRead(pedal_pin)?0:1;
    if (pedal_ && !last_pedal) {
      pos_ = 0;     //play
    }
    last_pedal = pedal_;
    
    //audio output
    if (pos_ < wave_n) {
      if (wave[pos_]) {
        digitalWrite(pin_buz, HIGH);    //buzzer ON
      }
      else {
        digitalWrite(pin_buz, LOW);    //buzzer OFF        
      }

      pos_++;
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
