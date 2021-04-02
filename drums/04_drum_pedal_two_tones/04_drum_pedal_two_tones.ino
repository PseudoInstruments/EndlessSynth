/* Endless Drums Pedal generates two drum sounds by pressing physical pedal or keys.

  Controller: Arduino Mega
  ----------------------------------------
  Connection
  ----------------------------------------
  1) Audio output: D2 (Sig) and D3 (Gnd)

  2) Pedal Sustain - D11 (Sig) and D12 (Gnd)

  3) Sliders - this sketch uses two sliders (I mean potentiometers 10kOhm) for controlling sound parameters.
  A4 - sample rate
  A5 - duration
  And for pots connect Gnd, 5V

  //It's better to use linear pots.
  ----------------------------------------

*/

const byte audio_pin = 2;     //Audio output Sig
const byte audio_gnd_pin = 3; //Audio output Gnd

const byte pedal_pin = 11;  //Pedal pin Sig
const byte pedal_gnd_pin = 12;  //Pedal pin Gnd

const byte led_pin = 13;  //Built-in led pin


//--------------------------------------------------------------
const byte Pots = 9;    //number of pots
//Pots values
int Pot[Pots] = {-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000};          
byte Pot_changed[Pots]; //is pot changed

int *Pot_Drum1_Duration = Pot[0];
int *Pot_Drum1_Timbre = Pot[1];
int *Pot_Drum1_Tone1 = Pot[2];
int *Pot_Drum1_Tone2 = Pot[3];

int *Pot_Sample_Rate = Pot[4];

int *Pot_Drum2_Duration = Pot[5];
int *Pot_Drum2_Timbre = Pot[6];
int *Pot_Drum2_Tone1 = Pot[7];
int *Pot_Drum2_Tone2 = Pot[8];

const byte pot_apin[Pots] = {A8,A9,A10,A11,A7,A6,A5,A4,A3};

const int pot_min = 0;     //for adjusting pots boundaries
const int pot_max = 1023;
const int pot_change_thresh = 10;  //value for set that pot is changed

//--------------------------------------------------------------

int debug = 0;    //control from keyboard to begin debugging

//--------------------------------------------------------------
//set 5V or Gnd to pin
void pin_power(int pin, int value) {
  pinMode(pin, OUTPUT);
  if (value) digitalWrite(pin, HIGH);
  else digitalWrite(pin, LOW);
}

//--------------------------------------------------------------
void setup() {
  Serial.begin(500000);
  Serial.println("Endless Drum Pedal Two Tones, v. 1.1 for Arduino Mega");
  Serial.println("Audio output: D2 (Sig), D3 (Gnd), pedal: D11, D12");
  Serial.println("TODO Slider 'Volume': D2, audio output, Gnd, Slider 'Sample' Rate: 5V, A4, Gnd, Slider 'Duration': 5V, A5, Gnd");
  Serial.println("Send '1' to enable debug print to console");

  //will be computed
  //Serial.print("Audio sample rate: "); Serial.println(audio_sample_rate);

  pinMode(audio_pin, OUTPUT); //activate audio output
  pin_power(audio_gnd_pin, 0);

  pinMode(pedal_pin, INPUT_PULLUP); //pedal
  pin_power(pedal_gnd_pin, 0);

  pinMode(led_pin, OUTPUT); //activate led

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
void read_pots() {
  int value;
  for (byte i=0; i<Pots; i++) {
    value = analogRead(pot_apin[i]);
    //Serial.print(i); Serial.print(" "); Serial.println(value);
    Pot_changed[i] = (abs(value - Pot[i]) > pot_change_thresh);
    Pot[i] = value;
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
  if (pedal_) digitalWrite(led_pin, HIGH);
  else digitalWrite(led_pin, LOW);

  //Pots
  read_pots();

  audio_delay_mcs = mapi_clamp(*Pot_Sample_Rate, pot_min, pot_max, audio_delay_mcs0, audio_delay_mcs1);

  wave_duration = mapi_clamp(*Pot_Drum1_Duration, pot_min, pot_max, wave_duration_min, wave_n);



  //debug print
  if (debug) {
    //use audio input for setting up trimmer resistor so it print 512
    Serial.print("pedal="); Serial.print(pedal_);
    Serial.print(" loops="); Serial.print(loops_);

    Serial.print(" Pots: "); 
    for (byte i=0; i<Pots; i++) {
      Serial.print(Pot[i]);
      if (Pot_changed[i]) Serial.print("+");
      else Serial.print(" ");
      Serial.print(" ");
    }
    Serial.print("  audio_delay_mcs="); Serial.print(audio_delay_mcs);
    Serial.print("  wave_duration="); Serial.print(wave_duration);

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
        digitalWrite(audio_pin, HIGH);    //buzzer ON
      }
      else {
        digitalWrite(audio_pin, LOW);    //buzzer OFF
      }

      pos_++;
    }
    else {
      pos_ = wave_n;                  //prevent continuation of sound play
      digitalWrite(audio_pin, LOW);    //buzzer OFF
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
