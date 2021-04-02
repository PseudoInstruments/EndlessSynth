/* Endless Drums Pedal generates drum sound by pressing pedal - physical Sustain pedal.

  Controller: Arduino Uno or Nano
  ----------------------------------------
  Connection
  ----------------------------------------
  1) Audio output: Gns and D2

  2) Pedal Sustain - Gnd and D11

  3) Sliders - this sketch uses two sliders (I mean potentiometers 10kOhm) for controlling sound parameters.
  A4 - sample rate
  A5 - duration
  And for pots connect Gnd, 5V

  //It's better to use linear pots.
  ----------------------------------------

*/

const byte pin_buz = 2; //Audio output pin
const byte pin_led = 13;  //Built-in led pin

const byte pedal_pin = 11;  //Pedal pin

//slider1 affects volume output directly, without arduino
const byte slider2_analog_pin = A4;   //sample rate
const byte slider3_analog_pin = A5;   //duration

const unsigned int analog_min = 20;     //it's appears minimal value is 20, not 0 in the current setup
const unsigned int analog_max = 1023;

int debug = 0;    //control from keyboard to begin debugging

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("Endless Drum Pedal, v. 1.1 for Arduino Uno, Nano");
  Serial.println("Audio output: pin D2, pedal: D11");
  Serial.println("Slider 'Volume': D2, audio output, Gnd, Slider 'Sample' Rate: 5V, A4, Gnd, Slider 'Duration': 5V, A5, Gnd");
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
//Pedal
byte pedal_ = 0;

//Drum sample
const int wave_n = 512;
byte wave[wave_n];   //TODO store in bits

//controllable
const int wave_duration_min = 10;
int wave_duration = wave_n; //0..wave_n

//sample pos
int pos_ = wave_n;

//--------------------------------------------------------------
//Sample rate
const int audio_delay_mcs0 = 0;
const int audio_delay_mcs1 = 4000;//400;   
int audio_delay_mcs = 10;   //delay in sound loop, controls sample rate


int loops_ = 300;


//--------------------------------------------------------------
void init_wave() {
  for (int i = 0; i < wave_n; i++) {
    wave[i] = (random(100) < 50) ? 0 : 1;
    //Serial.print(wave[i]);
    //Serial.print(" ");
  }

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

  int slider2 = analogRead(slider2_analog_pin);  //0..1023
  int slider3 = analogRead(slider3_analog_pin);  //0..1023

  audio_delay_mcs = mapi_clamp(slider2, analog_min, analog_max, audio_delay_mcs0, audio_delay_mcs1);

  wave_duration = mapi_clamp(slider3, analog_min, analog_max, wave_duration_min, wave_n);



  //debug print
  if (debug) {
    //use audio input for setting up trimmer resistor so it print 512
    Serial.print("pedal "); Serial.print(pedal_);
    Serial.print("  audio loops "); Serial.print(loops_);
    Serial.print("  sliders "); Serial.print(slider2);
    Serial.print(","); Serial.print(slider3);
    Serial.print("  audio_delay_mcs "); Serial.print(audio_delay_mcs);
    Serial.print("  wave_duration "); Serial.print(wave_duration);

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
    pedal_ = digitalRead(pedal_pin) ? 0 : 1;
    if (pedal_ && !last_pedal) {
      pos_ = 0;     //play
    }
    last_pedal = pedal_;

    //audio output
    if (pos_ < wave_duration) {
      if (wave[pos_]) {
        digitalWrite(pin_buz, HIGH);    //buzzer ON
      }
      else {
        digitalWrite(pin_buz, LOW);    //buzzer OFF
      }

      pos_++;
    }
    else {
      pos_ = wave_n;                  //prevent continuation of sound play
      digitalWrite(pin_buz, LOW);    //buzzer OFF
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
